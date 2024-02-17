#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "smtc_modem_api.h"
#include "smtc_modem_utilities.h"

#include "smtc_modem_hal.h"
#include "smtc_hal_dbg_trace.h"

#include "smtc_hal_mcu.h"
#include "smtc_hal_gpio.h"
#include "smtc_hal_rtc.h"

#include "ralf_lr11xx.h"
#include "lr11xx_wifi.h"
#include "lr11xx_gnss.h"

#include "string.h"

#include "settings.h"
#include "led.h"
#include "button.h"
#include "sht3x.h"
#include "lis2dw.h"
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define DEFAULT_UL_PORT         7
#define DEFAULT_DL_PORT         100

#define PERIODIC_TIMER_PERIOD 20000

#define LINK_CHECK_RATIO_THRESHOLD 50
#define LINK_CHECK_ATTEMPTS_THRESHOLD 10

/**
 * Stack id value (multistacks modem is not yet available)
 */
#define STACK_ID 0

const ralf_t modem_radio = RALF_LR11XX_INSTANTIATE( NULL );
#define RADIO   "LR11XX"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */
static volatile bool user_button_is_press = false;  // Flag for button status
static bool          periodic_message_flag;
static uint8_t       rx_payload[255]      = { 0 };  // Buffer for rx payload
static uint8_t       rx_payload_size      = 0;      // Size of the payload in the rx_payload buffer
extern settings_t    settings;
uint32_t txdone_counter = 0;
uint32_t link_check_attempts = 0;
static LedHandle_t led_fmlr, led_r, led_g, led_b;
static ButtonHandle_t button_handle;
static uint32_t user_button_state, tx_pending;
static int32_t  temp, rh;
static uint8_t wifi_count, gnss_count;
static int16_t acc_axes[3];
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */
static bool is_joined( void );
static void get_event( void );
static void wifi_scan( void );
static void init_func();

static TimerEvent_t periodic_timer;
static void periodic_timer_cb( void* context ) {
    SMTC_HAL_TRACE_PRINTF( "periodic_timer_cb\r\n" );
    periodic_message_flag = true;
    TimerStart( &periodic_timer );
}

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Example to send a user payload on an external event
 *
 */
int main( void ) {

    // Disable IRQ to avoid unwanted behaviour during init
    hal_mcu_disable_irq( );

    // Configure all the uC periph (clock, gpio, timer, ...)
    hal_mcu_init( );
    SMTC_HAL_TRACE_PRINTF("_______________________________________________________________________________\r\n");

    // Init the modem and use get_event as event callback, please note that the callback will be
    // called immediately after the first call to smtc_modem_run_engine because of the reset detection
//    SMTC_HAL_TRACE_PRINTF("calling: smtc_modem_init\n");
    smtc_modem_init( &modem_radio, &get_event );
    smtc_modem_version_t version;
    smtc_modem_get_modem_version( &version );

    hal_mcu_enable_irq( );
    init_func();



    //periodic timer example
    TimerInit( &periodic_timer, &periodic_timer_cb );
    TimerSetValue( &periodic_timer, PERIODIC_TIMER_PERIOD );
    TimerSetContext( &periodic_timer, NULL );
    TimerStart(&periodic_timer);

    while( 1 ) {
        wifi_scan();

        hal_mcu_delay_ms(2000);
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/**
 * @brief User callback for modem event
 *
 *  This callback is called every time an event ( see smtc_modem_event_t ) appears in the modem.
 *  Several events may have to be read from the modem when this callback is called.
 */
static void get_event( void ) {
    //SMTC_HAL_TRACE_MSG_COLOR( "get_event () callback\r\n", HAL_DBG_TRACE_COLOR_BLUE );

    // smtc_modem_event_t current_event;
    // uint8_t            event_pending_count;
    // uint8_t            stack_id = STACK_ID;

    // // Continue to read modem event until all event has been processed
    // do {
    //     // Read modem event
    //     smtc_modem_get_event( &current_event, &event_pending_count );

    //     switch( current_event.event_type ) {
    //     case SMTC_MODEM_EVENT_RESET:
    //         SMTC_HAL_TRACE_INFO( "Event received: RESET\r\n" );

    //         // Set user credentials
    //         smtc_modem_set_deveui( stack_id, settings.deveui );
    //         smtc_modem_set_joineui( stack_id, settings.joineui );
    //         smtc_modem_set_nwkkey( stack_id, settings.nwkey );
    //         // Set user region
    //         smtc_modem_set_region( stack_id, settings.region );
    //         // Schedule a Join LoRaWAN network
    //         smtc_modem_join_network( stack_id );
    //         break;

    //     case SMTC_MODEM_EVENT_ALARM:
    //         SMTC_HAL_TRACE_INFO( "Event received: ALARM\r\n" );

    //         break;

    //     case SMTC_MODEM_EVENT_JOINED:
    //         SMTC_HAL_TRACE_INFO( "Event received: JOINED\r\n" );
    //         SMTC_HAL_TRACE_INFO( "Modem is now joined \r\n" );

    //         SMTC_HAL_TRACE_INFO( "Send hello_world message \r\n" );
    //         char data[] = "hello_world";
    //         smtc_modem_request_uplink( STACK_ID, 102, false, ( uint8_t* )&data, sizeof( data ) );

    //         break;

    //     case SMTC_MODEM_EVENT_TXDONE:
    //         SMTC_HAL_TRACE_INFO( "Event received: TXDONE\r\n" );
    //         SMTC_HAL_TRACE_INFO( "Transmission done \r\n" );
    //         tx_pending = 0;
    //         break;

    //     case SMTC_MODEM_EVENT_DOWNDATA:
    //         SMTC_HAL_TRACE_INFO( "Event received: DOWNDATA\r\n" );
    //         rx_payload_size = ( uint8_t ) current_event.event_data.downdata.length;
    //         memcpy( rx_payload, current_event.event_data.downdata.data, rx_payload_size );
    //         SMTC_HAL_TRACE_PRINTF( "Data received on port %u\r\n", current_event.event_data.downdata.fport );
    //         SMTC_HAL_TRACE_ARRAY( "Received payload", rx_payload, rx_payload_size );
    //         break;

    //     case SMTC_MODEM_EVENT_UPLOADDONE:
    //         SMTC_HAL_TRACE_INFO( "Event received: UPLOADDONE\r\n" );

    //         break;

    //     case SMTC_MODEM_EVENT_SETCONF:
    //         SMTC_HAL_TRACE_INFO( "Event received: SETCONF\r\n" );

    //         break;

    //     case SMTC_MODEM_EVENT_MUTE:
    //         SMTC_HAL_TRACE_INFO( "Event received: MUTE\r\n" );

    //         break;

    //     case SMTC_MODEM_EVENT_STREAMDONE:
    //         SMTC_HAL_TRACE_INFO( "Event received: STREAMDONE\r\n" );

    //         break;

    //     // case SMTC_MODEM_EVENT_JOINFAIL:
    //     //     SMTC_HAL_TRACE_INFO( "Event received: JOINFAIL\r\n" );
    //     //     SMTC_HAL_TRACE_WARNING( "Join request failed \r\n" );
    //     //     break;

    //     case SMTC_MODEM_EVENT_TIME:
    //         SMTC_HAL_TRACE_INFO( "Event received: TIME\r\n" );
    //         break;

    //     case SMTC_MODEM_EVENT_TIMEOUT_ADR_CHANGED:
    //         SMTC_HAL_TRACE_INFO( "Event received: TIMEOUT_ADR_CHANGED\r\n" );
    //         break;

    //     case SMTC_MODEM_EVENT_NEW_LINK_ADR:
    //         SMTC_HAL_TRACE_INFO( "Event received: NEW_LINK_ADR\r\n" );
    //         break;

    //     case SMTC_MODEM_EVENT_LINK_CHECK:
    //         SMTC_HAL_TRACE_INFO( "Event received: LINK_CHECK\r\n" );

    //         //Link Check Example
    //         if( current_event.event_data.link_check.status == SMTC_MODEM_EVENT_LINK_CHECK_RECEIVED ) {
    //             SMTC_HAL_TRACE_INFO( "Link check successfull\r\n" );
    //             link_check_attempts = 0;
    //         } else {
    //             SMTC_HAL_TRACE_INFO( "Link check failed\r\n" );
    //             link_check_attempts++;

    //             if( link_check_attempts >= LINK_CHECK_ATTEMPTS_THRESHOLD ) {
    //                 SMTC_HAL_TRACE_INFO( "Link check failed %d times, rejoin network\r\n", link_check_attempts );
    //                 smtc_modem_leave_network( STACK_ID );
    //                 led_set( &led_b, LED_ON );
    //                 // TimerStop( &measurement_timer );
    //                 // TimerStop( &send_cyclic_data_timer );
    //                 smtc_modem_join_network( STACK_ID );
    //             }
    //         }

    //         break;

    //     case SMTC_MODEM_EVENT_ALMANAC_UPDATE:
    //         SMTC_HAL_TRACE_INFO( "Event received: ALMANAC_UPDATE\r\n" );
    //         break;

    //     case SMTC_MODEM_EVENT_USER_RADIO_ACCESS:
    //         SMTC_HAL_TRACE_INFO( "Event received: USER_RADIO_ACCESS\r\n" );
    //         break;

    //     case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_INFO:
    //         SMTC_HAL_TRACE_INFO( "Event received: CLASS_B_PING_SLOT_INFO\r\n" );
    //         break;

    //     case SMTC_MODEM_EVENT_CLASS_B_STATUS:
    //         SMTC_HAL_TRACE_INFO( "Event received: CLASS_B_STATUS\r\n" );
    //         break;

    //     default:
    //         SMTC_HAL_TRACE_ERROR( "Unknown event %u\r\n", current_event.event_type );
    //         break;
    //     }
    // } while( event_pending_count > 0 );
}

/**
 * @brief Join status of the product
 *
 * @return Return 1 if the device is joined else 0
 */
static bool is_joined( void ) {
    uint32_t status = 0;
    smtc_modem_get_status( STACK_ID, &status );

    if( ( status & SMTC_MODEM_STATUS_JOINED ) == SMTC_MODEM_STATUS_JOINED ) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief User callback for button EXTI
 *
 * @param context Define by the user at the init
 */
static void user_button_callback( void* context ) {
    SMTC_HAL_TRACE_INFO( "Button pushed\r\n" );
    led_set( &led_b, LED_OFF );
    led_blink( &led_b, 100, 1000 );
    ( void ) context;  // Not used in the example - avoid warning
}

/**
 * @brief Wi-Fi scanning function
 */
static void wifi_scan( void ) {
    SMTC_HAL_TRACE_PRINTF( "\r\n-----------------WIFI Scan-----------------\r\n" );
    uint32_t i;
    uint32_t wifi_counter = 0;

    lr11xx_wifi_extended_full_result_t wifi_scan_result;
    lr11xx_status_t ret;
    uint8_t nb_results;
    ret = lr11xx_wifi_scan( modem_radio.ral.context, LR11XX_WIFI_TYPE_SCAN_B_G_N, 0x3FFF, LR11XX_WIFI_SCAN_MODE_UNTIL_SSID, 12, 3, 110, true );
    hal_mcu_delay_ms( 110 );

    if( ret != LR11XX_STATUS_OK ) {
        SMTC_HAL_TRACE_PRINTF( "WIFI scan error\r\n" );
    }

    ret = lr11xx_wifi_get_nb_results( modem_radio.ral.context, &nb_results );

    if( nb_results > 0 ) {
        SMTC_HAL_TRACE_PRINTF( "Listing WIFI networks: %d\r\n", nb_results );
    } else {
        SMTC_HAL_TRACE_PRINTF( "No WIFI networks found: %d\r\n", nb_results );
    }

    for( i = wifi_counter; i < nb_results; i++ ) {
        ret = lr11xx_wifi_read_extended_full_results( modem_radio.ral.context, i, 1, &wifi_scan_result );
        SMTC_HAL_TRACE_PRINTF( "wifi %d: SSID: %s, ", i, wifi_scan_result.ssid_bytes );
        SMTC_HAL_TRACE_PRINTF( "mac: %02x:%02x:%02x:%02x:%02x:%02x, rssi: %d\r\n", wifi_scan_result.mac_address_3[0], wifi_scan_result.mac_address_3[1], wifi_scan_result.mac_address_3[2], wifi_scan_result.mac_address_3[3], wifi_scan_result.mac_address_3[4], wifi_scan_result.mac_address_3[5], wifi_scan_result.rssi );
    }

    if( ret != LR11XX_STATUS_OK ) {
        SMTC_HAL_TRACE_PRINTF( "LR1110 ERROR \r\n" );
    }

    wifi_counter = nb_results;
    wifi_count = nb_results;
}

static void init_func() {
    settings_init();
}
