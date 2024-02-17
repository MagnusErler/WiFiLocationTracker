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
