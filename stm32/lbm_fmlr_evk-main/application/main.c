
#include <stdbool.h>  // bool type

#include "smtc_hal_dbg_trace.h"

#include "ralf_lr11xx.h"
#include "lr11xx_wifi.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */


const ralf_t modem_radio = RALF_LR11XX_INSTANTIATE( NULL );


static void wifi_scan( void );


int main( void ) {

    hal_mcu_init( );
    SMTC_HAL_TRACE_PRINTF("_______________________________________________________________________________\r\n");

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

    lr11xx_wifi_extended_full_result_t wifi_scan_result;
    uint8_t nb_results;
    lr11xx_status_t ret = lr11xx_wifi_scan( modem_radio.ral.context, LR11XX_WIFI_TYPE_SCAN_B_G_N, 0x3FFF, LR11XX_WIFI_SCAN_MODE_UNTIL_SSID, 12, 3, 110, true );
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

    for( int i = 0; i < nb_results; i++ ) {
        ret = lr11xx_wifi_read_extended_full_results( modem_radio.ral.context, i, 1, &wifi_scan_result );
        SMTC_HAL_TRACE_PRINTF( "wifi %d: SSID: %s, ", i, wifi_scan_result.ssid_bytes );
        SMTC_HAL_TRACE_PRINTF( "mac: %02x:%02x:%02x:%02x:%02x:%02x, rssi: %d\r\n", wifi_scan_result.mac_address_3[0], wifi_scan_result.mac_address_3[1], wifi_scan_result.mac_address_3[2], wifi_scan_result.mac_address_3[3], wifi_scan_result.mac_address_3[4], wifi_scan_result.mac_address_3[5], wifi_scan_result.rssi );
    }

    if( ret != LR11XX_STATUS_OK ) {
        SMTC_HAL_TRACE_PRINTF( "LR1110 ERROR \r\n" );
    }
}
