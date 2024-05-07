/*!
 * \file      main_geolocation.c
 *
 * \brief     main program for GNSS & Wi-Fi geolocation example
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type
#include <string.h>

#include "main.h"

#include "smtc_modem_api.h"
#include "smtc_modem_geolocation_api.h"
#include "smtc_modem_utilities.h"

#include "smtc_modem_hal.h"
#include "smtc_hal_dbg_trace.h"

#include "example_options.h"

#include "smtc_hal_mcu.h"
#include "smtc_hal_gpio.h"
#include "smtc_hal_watchdog.h"

#include "lr11xx_system.h"

#include "lr1110_board.h"

#include "lis2de12.h"

#include "ral_lr11xx.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/**
 * @brief Returns the minimum value between a and b
 *
 * @param [in] a 1st value
 * @param [in] b 2nd value
 * @retval Minimum value
 */
#ifndef MIN
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/**
 * Stack id value (multistacks modem is not yet available)
 */
#define STACK_ID 0

/**
 * @brief Watchdog counter reload value during sleep (The period must be lower than MCU watchdog period (here 32s))
 */
#define WATCHDOG_RELOAD_PERIOD_MS ( 20000 )

#define CUSTOM_NB_TRANS ( 3 )
#define ADR_CUSTOM_LIST                                \
    {                                                  \
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 \
    }

static const uint8_t adr_custom_list[16] = ADR_CUSTOM_LIST;
static const uint8_t custom_nb_trans     = CUSTOM_NB_TRANS;

#define KEEP_ALIVE_PORT ( 2 )
#define KEEP_ALIVE_PERIOD_S ( 3600 / 2 )
#define KEEP_ALIVE_SIZE ( 4 )
uint8_t keep_alive_payload[KEEP_ALIVE_SIZE] = { 0x00 };

/*!
 * @brief Defines the delay before starting the next scan sequence, value in [s].
 */
uint8_t GEOLOCATION_GNSS_SCAN_PERIOD_S = 2 * 60;
uint8_t GEOLOCATION_WIFI_SCAN_PERIOD_S = 1 * 15;

/*!
 * @brief Time during which a LED is turned on when pulse, in [ms]
 */
#define LED_PERIOD_MS ( 250 )

/**
 * @brief Supported LR11XX radio firmware
 */
#define LR1110_FW_VERSION 0x0401

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */
static uint8_t                  rx_payload[SMTC_MODEM_MAX_LORAWAN_PAYLOAD_LENGTH] = { 0 };  // Buffer for rx payload
static uint8_t                  rx_payload_size = 0;      // Size of the payload in the rx_payload buffer
static smtc_modem_dl_metadata_t rx_metadata     = { 0 };  // Metadata of downlink
static uint8_t                  rx_remaining    = 0;      // Remaining downlink payload in modem

/**
 * @brief Internal credentials
 */
static uint8_t join_eui[SMTC_MODEM_EUI_LENGTH] = { 0 };
static uint8_t chip_eui[SMTC_MODEM_EUI_LENGTH] = { 0 };
static uint8_t pin[SMTC_MODEM_PIN_LENGTH] = { 0 };

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/**
 * @brief User callback for modem event
 *
 *  This callback is called every time an event ( see smtc_modem_event_t ) appears in the modem.
 *  Several events may have to be read from the modem when this callback is called.
 */
static void modem_event_callback( void );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Example to send a user payload on an external event
 *
 */
void main_geolocation( void ) {
    uint32_t                sleep_time_ms = 0;
    lr11xx_system_version_t lr11xx_fw_version;
    // lr11xx_status_t         status;

    // Disable IRQ to avoid unwanted behaviour during init
    hal_mcu_disable_irq( );

    // Configure all the µC periph (clock, gpio, timer, ...)
    hal_mcu_init( );

    accelerometer_init( );

    // Init the modem and use modem_event_callback as event callback, please note that the callback will be
    // called immediately after the first call to smtc_modem_run_engine because of the reset detection
    smtc_modem_init( &modem_event_callback );

    // Re-enable IRQ
    hal_mcu_enable_irq( );

    SMTC_HAL_TRACE_PRINTF("\r\n\r\n-----------------------------\r\n\r\n" );

    SMTC_HAL_TRACE_INFO( "GEOLOCATION example is starting\r\n" );

    ral_lr11xx_init( NULL );

    /* Check LR11XX Firmware version */
    if( lr11xx_system_get_version( NULL, &lr11xx_fw_version ) != LR11XX_STATUS_OK ) {
        SMTC_HAL_TRACE_ERROR( "Failed to get LR11XX firmware version\r\n" );
    }
    if( ( lr11xx_fw_version.type == LR11XX_SYSTEM_VERSION_TYPE_LR1110 ) && ( lr11xx_fw_version.fw < LR1110_FW_VERSION ) ) {
        SMTC_HAL_TRACE_ERROR( "Wrong LR1110 firmware version, expected 0x%04X, got 0x%04X\r\n", LR1110_FW_VERSION, lr11xx_fw_version.fw );
    }
    SMTC_HAL_TRACE_INFO( "LR11XX FW: 0x%04X, type: 0x%02X\r\n", lr11xx_fw_version.fw, lr11xx_fw_version.type );
    SMTC_HAL_TRACE_INFO( "LR11XX HW: 0x%02X\r\n", lr11xx_fw_version.hw );

    while( 1 ) {

        // uint16_t temp_10_0;
        // lr11xx_system_get_temp( NULL, &temp_10_0 );
        // const float temperature = 25 + (1000/(-1.7)) * ((temp_10_0/2047.0) * 1.35 - 0.7295);
        // SMTC_HAL_TRACE_INFO("%d.%d °C\r\n", (uint8_t)temperature, (uint8_t)((temperature - (uint8_t)temperature) * 100));
        // if ((uint8_t)temperature > 50) {
        //     SMTC_HAL_TRACE_INFO("LR1110 temperature is too high. TCXO mode may not be set up correctly\r\n");
        // }

        // uint8_t vbat;
        // lr11xx_system_get_vbat( NULL, &vbat );
        // const float batteryVoltage = (((5 * vbat)/255.0) - 1) * 1.35;
        // SMTC_HAL_TRACE_INFO("%d.%d V\r\n", (uint8_t)batteryVoltage, (uint8_t)((batteryVoltage - (uint8_t)batteryVoltage) * 100));

        // SMTC_HAL_TRACE_INFO("LIS2DE12 Temperature: %d\r\n", acc_get_temperature( ));
        // acc_read_raw_data( );
        // SMTC_HAL_TRACE_INFO("X: %d, Y: %d, Z: %d\r\n", acc_get_raw_x( ), acc_get_raw_y( ), acc_get_raw_z( ));

        if (get_accelerometer_irq1_state( ) == 1) {
            SMTC_HAL_TRACE_INFO("LIS2DE12 interrupt 1 detected\r\n");
            is_accelerometer_detected_moved( );

            lis2de12_int1_src_t int1_src = { 0 };
            if (lis2de12_int1_gen_source_get( &int1_src ) != 0 ) {
                SMTC_HAL_TRACE_ERROR("LIS2DE12 INT1_SRC: Error reading INT1_SRC register\r\n");
            } else {
                if (int1_src.ia == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: One or more interrupts have been generated\r\n");
                }
                if (int1_src.xl == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: X low event has occurred\r\n");
                }
                if (int1_src.xh == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: X high event has occurred\r\n");
                }
                if (int1_src.yl == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: Y low event has occurred\r\n");
                }
                if (int1_src.yh == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: Y high event has occurred\r\n");
                }
                if (int1_src.zl == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: Z low event has occurred\r\n");
                }
                if (int1_src.zh == 1) {
                    SMTC_HAL_TRACE_INFO("LIS2DE12 INT1_SRC: Z high event has occurred\r\n");
                }
            }
            
        }


        // Modem process launch
        sleep_time_ms = smtc_modem_run_engine( );

        // Atomically check sleep conditions
        hal_mcu_disable_irq( );
        if( smtc_modem_is_irq_flag_pending( ) == false ) {

            hal_watchdog_reload( );
            hal_mcu_set_sleep_for_ms( MIN( sleep_time_ms, WATCHDOG_RELOAD_PERIOD_MS ) );
        }
        hal_watchdog_reload( );
        hal_mcu_enable_irq( );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

void setupWiFi(uint8_t stack_id) {
    smtc_modem_wifi_send_mode( stack_id, SMTC_MODEM_SEND_MODE_UPLINK );
    /* Program Wi-Fi scan */
    smtc_modem_wifi_scan( stack_id, 0 );
}

void setupGNSS(uint8_t stack_id) {
    /* Start almanac demodulation service */
    smtc_modem_almanac_demodulation_set_constellations( stack_id, SMTC_MODEM_GNSS_CONSTELLATION_GPS_BEIDOU );
    smtc_modem_almanac_demodulation_start( stack_id );

    smtc_modem_gnss_send_mode( stack_id, SMTC_MODEM_SEND_MODE_STORE_AND_FORWARD );
    /* Program GNSS scan */
    smtc_modem_gnss_set_constellations( stack_id, SMTC_MODEM_GNSS_CONSTELLATION_GPS_BEIDOU );
    smtc_modem_gnss_scan( stack_id, SMTC_MODEM_GNSS_MODE_MOBILE, 0 );
}

float getTemperature() {
    // SMTC_HAL_TRACE_INFO("LIS2DE12 Temperature: %d\r\n", acc_get_temperature( ));
    uint16_t temp_10_0;
    lr11xx_system_get_temp( NULL, &temp_10_0 );
    const float temperature = 25 + (1000/(-1.7)) * ((temp_10_0/2047.0) * 1.35 - 0.7295);
    SMTC_HAL_TRACE_INFO("%d.%d °C\r\n", (uint8_t)temperature, (uint8_t)((temperature - (uint8_t)temperature) * 100));
    if ((uint8_t)temperature > 50) {
        SMTC_HAL_TRACE_INFO("LR1110 temperature is too high. TCXO mode may not be set up correctly\r\n");
        return 0;
    }
    return temperature;
}

float getBatteryVoltage() {
    uint8_t vbat;
    lr11xx_system_get_vbat( NULL, &vbat );
    const float batteryVoltage = (((5 * vbat)/255.0) - 1) * 1.35;
    SMTC_HAL_TRACE_INFO("%d.%d V\r\n", (uint8_t)batteryVoltage, (uint8_t)((batteryVoltage - (uint8_t)batteryVoltage) * 100));
    return batteryVoltage;
}

/**
 * @brief User callback for modem event
 *
 *  This callback is called every time an event ( see smtc_modem_event_t ) appears in the modem.
 *  Several events may have to be read from the modem when this callback is called.
 */
static void modem_event_callback( void ) {
    SMTC_HAL_TRACE_MSG_COLOR( "get_event () callback\r\n", HAL_DBG_TRACE_COLOR_BLUE );

    smtc_modem_event_t                                          current_event;
    uint8_t                                                     event_pending_count;
    uint8_t                                                     stack_id = STACK_ID;
    smtc_modem_gnss_event_data_scan_done_t                      scan_done_data;
    smtc_modem_gnss_event_data_terminated_t                     terminated_data;
    smtc_modem_almanac_demodulation_event_data_almanac_update_t almanac_update_data;
    smtc_modem_wifi_event_data_scan_done_t                      wifi_scan_done_data;
    smtc_modem_wifi_event_data_terminated_t                     wifi_terminated_data;

    // Continue to read modem event until all event has been processed
    do {
        // Read modem event
        smtc_modem_get_event( &current_event, &event_pending_count );

        switch( current_event.event_type ) {
        case SMTC_MODEM_EVENT_RESET:
            SMTC_HAL_TRACE_INFO( "Event received: RESET\r\n" );

            // Get internal credentials
            smtc_modem_get_joineui( stack_id, join_eui );
            SMTC_HAL_TRACE_ARRAY( "JOIN_EUI", join_eui, SMTC_MODEM_EUI_LENGTH );
            smtc_modem_get_chip_eui( stack_id, chip_eui );
            SMTC_HAL_TRACE_ARRAY( "CHIP_EUI", chip_eui, SMTC_MODEM_EUI_LENGTH );
            smtc_modem_get_pin( stack_id, pin );
            SMTC_HAL_TRACE_ARRAY( "PIN", pin, SMTC_MODEM_PIN_LENGTH );

            /* Set user region */
            smtc_modem_set_region( stack_id, MODEM_EXAMPLE_REGION );
            /* Schedule a Join LoRaWAN network */
            smtc_modem_join_network( stack_id );

            /* Set GNSS and Wi-Fi send mode */
            smtc_modem_store_and_forward_flash_clear_data( stack_id );
            smtc_modem_store_and_forward_set_state( stack_id, SMTC_MODEM_STORE_AND_FORWARD_ENABLE );
            
            switch (1) {
            case 1:
                // WiFi scan first, then GNSS scan
                setupWiFi( stack_id );
                // setupGNSS( stack_id );
                break;
            case 2:
                setupGNSS( stack_id );
                setupWiFi( stack_id );
                break;
            default:
                break;
            }
            
            /* Notify user with leds */
            smtc_board_led_set( smtc_board_get_led_tx_mask( ), true );
            break;

        case SMTC_MODEM_EVENT_ALARM:
            SMTC_HAL_TRACE_INFO( "Event received: ALARM\r\n" );
            smtc_modem_request_uplink( stack_id, KEEP_ALIVE_PORT, false, keep_alive_payload, KEEP_ALIVE_SIZE );
            smtc_modem_alarm_start_timer( KEEP_ALIVE_PERIOD_S );
            break;

        case SMTC_MODEM_EVENT_JOINED:
            SMTC_HAL_TRACE_INFO( "Event received: JOINED\r\n" );
            /* Notify user with leds */
            smtc_board_led_set( smtc_board_get_led_tx_mask( ), false );
            smtc_board_led_pulse( smtc_board_get_led_rx_mask( ), true, LED_PERIOD_MS );
            /* Set custom ADR profile for geolocation */
            smtc_modem_adr_set_profile( stack_id, SMTC_MODEM_ADR_PROFILE_CUSTOM, adr_custom_list );
            smtc_modem_set_nb_trans( stack_id, custom_nb_trans );
            /* Start time for regular uplink */
            smtc_modem_alarm_start_timer( KEEP_ALIVE_PERIOD_S );
            break;

        case SMTC_MODEM_EVENT_TXDONE:
            SMTC_HAL_TRACE_INFO( "Event received: TXDONE (%d)\r\n", current_event.event_data.txdone.status );
            SMTC_HAL_TRACE_INFO( "Transmission done\r\n" );
            break;

        case SMTC_MODEM_EVENT_DOWNDATA:
            SMTC_HAL_TRACE_INFO( "Event received: DOWNDATA\r\n" );
            // Get downlink data

            // reset rx_payload
            memset( rx_payload, 0, SMTC_MODEM_MAX_LORAWAN_PAYLOAD_LENGTH );
            smtc_modem_get_downlink_data( rx_payload, &rx_payload_size, &rx_metadata, &rx_remaining );
            SMTC_HAL_TRACE_PRINTF( "Data received on port %u\r\n", rx_metadata.fport );
            SMTC_HAL_TRACE_ARRAY( "Received payload", rx_payload, rx_payload_size );


            uint8_t custom_payload[2] = { 0x00 };

            switch (rx_metadata.fport) {
            case 1:

                if (rx_payload_size == 1) {
                    SMTC_HAL_TRACE_PRINTF( "payload in dec: %u\r\n", rx_payload[0]);
                    GEOLOCATION_WIFI_SCAN_PERIOD_S = rx_payload[0];
                } else if (rx_payload_size == 2) {
                    SMTC_HAL_TRACE_PRINTF( "payload in dec: %u\r\n", (rx_payload[0] << 8) + rx_payload[1]);
                    GEOLOCATION_WIFI_SCAN_PERIOD_S = (rx_payload[0] << 8) + rx_payload[1];
                }

                SMTC_HAL_TRACE_PRINTF("Setting new Wi-Fi scan period to %u s\r\n", GEOLOCATION_WIFI_SCAN_PERIOD_S);

                // cancel the current Wi-Fi scan (not possible if the scan has already started)
                smtc_modem_wifi_scan_cancel( stack_id );
                // set the new Wi-Fi scan period
                smtc_modem_wifi_scan( stack_id, GEOLOCATION_WIFI_SCAN_PERIOD_S );


                // // smtc_modem_request_uplink( stack_id, 1, false, keep_alive_payload, KEEP_ALIVE_SIZE );
                // uint8_t custom_payload[4] = { 0x00 };
                // custom_payload[0] = 0xAB;
                // custom_payload[1] = 0xCD;
                // smtc_modem_request_uplink( stack_id, 15, false, custom_payload, 4 );
                // smtc_modem_alarm_start_timer( KEEP_ALIVE_PERIOD_S );
                break;
            case 2:

                if (rx_payload_size == 1) {
                    SMTC_HAL_TRACE_PRINTF( "payload in dec: %u\r\n", rx_payload[0]);
                    GEOLOCATION_GNSS_SCAN_PERIOD_S = rx_payload[0];
                } else if (rx_payload_size == 2) {
                    SMTC_HAL_TRACE_PRINTF( "payload in dec: %u\r\n", (rx_payload[0] << 8) + rx_payload[1]);
                    GEOLOCATION_GNSS_SCAN_PERIOD_S = (rx_payload[0] << 8) + rx_payload[1];
                }

                SMTC_HAL_TRACE_PRINTF("Setting new GNSS scan period to %u s\r\n", GEOLOCATION_GNSS_SCAN_PERIOD_S);

                // cancel the current GNSS scan (not possible if the scan has already started)
                smtc_modem_gnss_scan_cancel( stack_id );
                // set the new GNSS scan period
                smtc_modem_gnss_scan( stack_id, SMTC_MODEM_GNSS_MODE_MOBILE, GEOLOCATION_GNSS_SCAN_PERIOD_S );
                break;
            case 3:              
                // smtc_modem_request_uplink( stack_id, 1, false, keep_alive_payload, KEEP_ALIVE_SIZE );
                custom_payload[0] = getTemperature() / 5.0;
                custom_payload[1] = getBatteryVoltage() * 70;
                smtc_modem_request_uplink( stack_id, 15, false, custom_payload, 4 );
                smtc_modem_alarm_start_timer( KEEP_ALIVE_PERIOD_S );

                break;
            default:
                break;
            }

            break;

        case SMTC_MODEM_EVENT_JOINFAIL:
            SMTC_HAL_TRACE_INFO( "Event received: JOINFAIL\r\n" );
            SMTC_HAL_TRACE_WARNING( "Join request failed\r\n" );
            break;

        case SMTC_MODEM_EVENT_ALCSYNC_TIME:
            SMTC_HAL_TRACE_INFO( "Event received: TIME\r\n" );
            break;

        case SMTC_MODEM_EVENT_GNSS_SCAN_DONE:
            SMTC_HAL_TRACE_INFO( "Event received: GNSS_SCAN_DONE\r\n" );
            /* Get event data */
            smtc_modem_gnss_get_event_data_scan_done( stack_id, &scan_done_data );
            break;

        case SMTC_MODEM_EVENT_GNSS_TERMINATED:
            SMTC_HAL_TRACE_INFO( "Event received: GNSS_TERMINATED\r\n" );
            /* Notify user with leds */
            smtc_board_led_pulse( smtc_board_get_led_tx_mask( ), true, LED_PERIOD_MS );
            /* Get event data */
            smtc_modem_gnss_get_event_data_terminated( stack_id, &terminated_data );
            /* launch next scan */
            smtc_modem_gnss_scan( stack_id, SMTC_MODEM_GNSS_MODE_MOBILE, GEOLOCATION_GNSS_SCAN_PERIOD_S );
            break;

        case SMTC_MODEM_EVENT_GNSS_ALMANAC_DEMOD_UPDATE:
            SMTC_HAL_TRACE_INFO( "Event received: GNSS_ALMANAC_DEMOD_UPDATE\r\n" );
            smtc_modem_almanac_demodulation_get_event_data_almanac_update( stack_id, &almanac_update_data );
            /* Store progress in keep alive payload */
            keep_alive_payload[0] = almanac_update_data.update_progress_gps;
            keep_alive_payload[1] = almanac_update_data.update_progress_beidou;
            SMTC_HAL_TRACE_PRINTF( "GPS progress: %u%%\r\n", almanac_update_data.update_progress_gps );
            SMTC_HAL_TRACE_PRINTF( "BDS progress: %u%%\r\n", almanac_update_data.update_progress_beidou );
            SMTC_HAL_TRACE_PRINTF( "aborted: %u\r\n", almanac_update_data.stat_nb_aborted_by_rp );
            break;

        case SMTC_MODEM_EVENT_WIFI_SCAN_DONE:
            SMTC_HAL_TRACE_INFO( "Event received: WIFI_SCAN_DONE\r\n" );
            /* Get event data */
            smtc_modem_wifi_get_event_data_scan_done( stack_id, &wifi_scan_done_data );
            break;

        case SMTC_MODEM_EVENT_WIFI_TERMINATED:
            SMTC_HAL_TRACE_INFO( "Event received: WIFI_TERMINATED\r\n" );
            /* Notify user with leds */
            smtc_board_led_pulse( smtc_board_get_led_tx_mask( ), true, LED_PERIOD_MS );
            /* Get event data */
            smtc_modem_wifi_get_event_data_terminated( stack_id, &wifi_terminated_data );
            /* launch next scan */
            smtc_modem_wifi_scan( stack_id, GEOLOCATION_WIFI_SCAN_PERIOD_S );
            break;

        case SMTC_MODEM_EVENT_LINK_CHECK:
            SMTC_HAL_TRACE_INFO( "Event received: LINK_CHECK\r\n" );
            break;

        case SMTC_MODEM_EVENT_CLASS_B_STATUS:
            SMTC_HAL_TRACE_INFO( "Event received: CLASS_B_STATUS\r\n" );
            break;

        default:
            SMTC_HAL_TRACE_ERROR( "Unknown event %u\r\n", current_event.event_type );
            break;
        }
    } while( event_pending_count > 0 );
}

/* --- EOF ------------------------------------------------------------------ */
