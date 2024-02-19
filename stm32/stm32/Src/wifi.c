
#include "wifi.h"

#include "spi.h"

#include "main.h"   // for HAL_DBG_TRACE-functions

#include "led.h"

#include <stdint.h>
#include <stdbool.h>

static lr1110_wifi_status_t _lr11xx_wifi_read_results_helper( const void* context, const uint8_t start_index, const uint8_t n_elem, uint8_t* buffer, const lr11xx_wifi_result_format_t result_format ) {
    const uint8_t  size_single_elem   = 79;
    const uint8_t  result_format_code = 0x01;
    const uint8_t  cbuffer[2+3] = { ( uint8_t ) ( 0x0306 >> 8 ), ( uint8_t ) ( 0x0306 & 0x00FF ), start_index, n_elem, result_format_code };
    const uint16_t size_total                                  = n_elem * size_single_elem;
    return (lr1110_wifi_status_t) lr1110_spi_read( context, cbuffer, 2+3, buffer, size_total );
}

static void _lr11xx_wifi_read_mac_address_from_buffer( const uint8_t* buffer, const uint16_t index_in_buffer, lr11xx_wifi_mac_address_t mac_address ) {
    for( uint8_t field_mac_index = 0; field_mac_index < 6; field_mac_index++ ) {
        mac_address[field_mac_index] = buffer[index_in_buffer + field_mac_index];
    }
}

static uint16_t _uint16_from_array( const uint8_t* array, const uint16_t index ) {
    return ( uint16_t ) ( array[index] << 8 ) + ( ( uint16_t ) ( array[index + 1] ) );
}

static uint64_t _uint64_from_array( const uint8_t* array, const uint16_t index ) {
    return ( ( uint64_t ) ( array[index] ) << 56 ) + ( ( uint64_t ) ( array[index + 1] ) << 48 ) +
           ( ( uint64_t ) ( array[index + 2] ) << 40 ) + ( ( uint64_t ) ( array[index + 3] ) << 32 ) +
           ( ( uint64_t ) ( array[index + 4] ) << 24 ) + ( ( uint64_t ) ( array[index + 5] ) << 16 ) +
           ( ( uint64_t ) ( array[index + 6] ) << 8 ) + ( uint64_t ) ( array[index + 7] );
}

static void _interpret_extended_full_result_from_buffer( const uint8_t nb_results, const uint8_t index_result_start_writing, const uint8_t* buffer, lr11xx_wifi_extended_full_result_t* result ) {
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ ) {
        const uint16_t local_index_start = 79 * result_index;
        lr11xx_wifi_extended_full_result_t* local_wifi_result = &result[index_result_start_writing + result_index];

        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        local_wifi_result->rate                = buffer[local_index_start + 3];
        local_wifi_result->service             = _uint16_from_array( buffer, local_index_start + 4 );
        local_wifi_result->length              = _uint16_from_array( buffer, local_index_start + 6 );
        local_wifi_result->frame_control       = _uint16_from_array( buffer, local_index_start + 8 );
        _lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 10, local_wifi_result->mac_address_1 );
        _lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 16, local_wifi_result->mac_address_2 );
        _lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 22, local_wifi_result->mac_address_3 );
        local_wifi_result->timestamp_us     = _uint64_from_array( buffer, local_index_start + 28 );
        local_wifi_result->beacon_period_tu = _uint16_from_array( buffer, local_index_start + 36 );
        local_wifi_result->seq_control      = _uint16_from_array( buffer, local_index_start + 38 );
        for( uint8_t ssid_index = 0; ssid_index < 32; ssid_index++ ) {
            local_wifi_result->ssid_bytes[ssid_index] = buffer[local_index_start + ssid_index + 40];
        }
        local_wifi_result->current_channel               = buffer[local_index_start + 72];
        local_wifi_result->country_code[0]               = buffer[local_index_start + 73];
        local_wifi_result->country_code[1]               = buffer[local_index_start + 74];
        local_wifi_result->io_regulation                 = buffer[local_index_start + 75];
        local_wifi_result->fcs_check_byte.is_fcs_checked = ( ( buffer[local_index_start + 76] & 0x01 ) == 0x01 );
        local_wifi_result->fcs_check_byte.is_fcs_ok      = ( ( buffer[local_index_start + 76] & 0x02 ) == 0x02 );
        local_wifi_result->phi_offset                    = _uint16_from_array( buffer, local_index_start + 77 );
    }
}

static void _generic_results_interpreter( const uint8_t n_result_to_parse, const uint8_t index_result_start_writing, const uint8_t* buffer, lr11xx_wifi_result_interface_t result_interface, const lr11xx_wifi_result_format_t format_code ) {
    _interpret_extended_full_result_from_buffer( n_result_to_parse, index_result_start_writing, buffer,
                                                result_interface.extended_complete );
}

static uint8_t _min( const uint8_t a, const uint8_t b ) {
    return ( a < b ) ? a : b;
}

static lr1110_wifi_status_t _fetch_and_aggregate_all_results( const void* context, const uint8_t index_result_start, const uint8_t nb_results, const uint8_t nb_results_per_chunk_max, const lr11xx_wifi_result_format_t result_format_code, uint8_t* result_buffer, lr11xx_wifi_result_interface_t result_structures ) {
    uint8_t index_to_read              = index_result_start;
    uint8_t index_result_start_writing = 0;
    uint8_t remaining_results          = nb_results;

    while( remaining_results > 0 ) {
        uint8_t results_to_read = _min( remaining_results, nb_results_per_chunk_max );

        lr1110_wifi_status_t local_hal_status = _lr11xx_wifi_read_results_helper( context, index_to_read, results_to_read,
                                                                                result_buffer, result_format_code );
        if( local_hal_status != LR1110_WIFI_STATUS_OK ) {
            return ( lr1110_wifi_status_t ) local_hal_status;
        }

        _generic_results_interpreter( results_to_read, index_result_start_writing, result_buffer, result_structures,
                                     result_format_code ); {
        // Reset the content of the result_buffer in case there are still results to fetch
            for( uint16_t index = 0; index < 948; index++ ) {
                result_buffer[index] = 0;
            }
        }

        index_to_read += results_to_read;
        index_result_start_writing += results_to_read;
        remaining_results -= results_to_read;
    }
    return ( lr1110_wifi_status_t ) LR1110_WIFI_STATUS_OK;
}

static lr1110_wifi_status_t getWiFiFullResults( const void* context, const uint8_t start_result_index, const uint8_t nb_results, lr11xx_wifi_extended_full_result_t* results ) {
    uint8_t result_buffer1[1020]            = { 0 };
    const uint8_t nb_results_per_chunk_max  = 12;

    lr11xx_wifi_result_interface_t result_interface = { 0 };
    result_interface.extended_complete = results;

    return _fetch_and_aggregate_all_results( context, start_result_index, nb_results, nb_results_per_chunk_max, LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL, result_buffer1, result_interface );
}

lr1110_wifi_status_t getWiFiNbResults( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting number of WiFi networks found... ");

    uint8_t cbuffer[LR1110_NB_RESULTS_WIFI_CMD_LENGTH];
    uint8_t rbuffer[LR1110_NB_RESULTS_WIFI_LENGTH] = { 0 };

    cbuffer[0] = ( uint8_t ) LR1110_GROUP_ID_WIFI;
    cbuffer[1] = ( uint8_t ) 0x05;

    if ( lr1110_spi_read( context, cbuffer, LR1110_NB_RESULTS_WIFI_CMD_LENGTH, rbuffer, LR1110_NB_RESULTS_WIFI_LENGTH ) == LR1110_SPI_STATUS_OK ) {
        uint8_t nb_results = rbuffer[0];
        HAL_DBG_TRACE_INFO_VALUE("%d\r\n", nb_results);

        lr11xx_wifi_extended_full_result_t wifi_scan_result;
        for( int i = 0; i < nb_results; i++ ) {
            if (getWiFiFullResults( context, i, 1, &wifi_scan_result ) != LR1110_WIFI_STATUS_OK) {
                HAL_DBG_TRACE_ERROR( "Failed to read WiFi networks\r\n" );
                return LR1110_WIFI_STATUS_ERROR;
            }
            HAL_DBG_TRACE_INFO( "WiFi %d: MAC: %02x:%02x:%02x:%02x:%02x:%02x, RSSI: %d, SSID: %s\r\n", i, wifi_scan_result.mac_address_3[0], wifi_scan_result.mac_address_3[1], wifi_scan_result.mac_address_3[2], wifi_scan_result.mac_address_3[3], wifi_scan_result.mac_address_3[4], wifi_scan_result.mac_address_3[5], wifi_scan_result.rssi, wifi_scan_result.ssid_bytes );
        }
        return LR1110_WIFI_STATUS_OK;
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get WiFi networks count\r\n");
        return LR1110_WIFI_STATUS_ERROR;
    }
}

lr1110_wifi_status_t scanWiFiNetworks( const void* context, const lr11xx_wifi_signal_type_scan_t signal_type, const lr11xx_wifi_channel_mask_t chan_mask, const lr11xx_wifi_mode_t acq_mode, const uint8_t nb_max_res, const uint8_t nb_scan_per_chan, const uint16_t timeout, const bool abort_on_timeout ) {
    HAL_DBG_TRACE_INFO("Scanning WiFi networks... ");

    uint8_t cbuffer[LR1110_SCAN_WIFI_CMD_LENGTH];

    cbuffer[0] = ( uint8_t ) LR1110_GROUP_ID_WIFI;
    cbuffer[1] = ( uint8_t ) 0x00;
    cbuffer[2] = ( uint8_t ) signal_type;
    cbuffer[3] = ( uint8_t ) ( chan_mask >> 8 );
    cbuffer[4] = ( uint8_t ) ( chan_mask >> 0 );
    cbuffer[5] = ( uint8_t ) acq_mode;
    cbuffer[6] = nb_max_res;
    cbuffer[7] = nb_scan_per_chan;
    cbuffer[8] = ( uint8_t ) ( timeout >> 8 );
    cbuffer[9] = ( uint8_t ) ( timeout >> 0 );
    cbuffer[10] = ( uint8_t ) ( ( abort_on_timeout == true ) ? 1 : 0 );

    if ( lr1110_spi_write( context, cbuffer, LR1110_SCAN_WIFI_CMD_LENGTH ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

        HAL_Delay( 100 + timeout ); // wait for the scan to complete

        return LR1110_WIFI_STATUS_OK;
    } else {
        HAL_DBG_TRACE_ERROR("Failed to scan WiFi networks\r\n");
        return LR1110_WIFI_STATUS_ERROR;
    }
}

lr1110_wifi_status_t getLR1110_WiFi_Version( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting WiFi firmware version... ");

    uint8_t cbuffer[LR1110_WIFI_VERSION_CMD_LENGTH];
    uint8_t rbuffer[LR1110_WIFI_VERSION_LENGTH] = { 0 };

    cbuffer[0] = ( uint8_t ) LR1110_GROUP_ID_WIFI;
    cbuffer[1] = ( uint8_t ) LR1110_WIFI_GET_FIRMWARE_WIFI_VERSION_CMD;

    turnOnLED(SNIFFING_LED_GPIO_Port, SNIFFING_LED_Pin);
    if (lr1110_spi_read( context, cbuffer, LR1110_WIFI_VERSION_CMD_LENGTH, rbuffer, LR1110_WIFI_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
        turnOffLED(SNIFFING_LED_GPIO_Port, SNIFFING_LED_Pin);
        HAL_DBG_TRACE_INFO_VALUE("%d.%d (0x%X.0x%X)\r\n", rbuffer[0], rbuffer[1], rbuffer[0], rbuffer[1]);
        return LR1110_WIFI_STATUS_OK;
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get WiFi firmware version\r\n");
        return LR1110_WIFI_STATUS_ERROR;
    }
}