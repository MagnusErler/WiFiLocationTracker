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

#include "string.h"

#include "settings.h"
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */


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

static uint8_t wifi_count;
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */
static void wifi_scan( void );

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

    hal_mcu_enable_irq( );

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
