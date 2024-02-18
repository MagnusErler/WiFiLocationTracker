


#include "wifi.h"

#include "spi.h"

#include "main.h"   // for HAL_DBG_TRACE-functions

#include <stdint.h>
#include <stdbool.h>



lr11xx_status_t1 lr11xx_wifi_scan( const void* context, const lr11xx_wifi_signal_type_scan_t1 signal_type,
                                  const lr11xx_wifi_channel_mask_t1 channels, const lr11xx_wifi_mode_t1 scan_mode,
                                  const uint8_t max_results, const uint8_t nb_scan_per_channel,
                                  const uint16_t timeout_in_ms, const bool abort_on_timeout ) {
    const uint8_t cbuffer[2+9] = {
        ( uint8_t ) ( 0x0300 >> 8 ),
        ( uint8_t ) ( 0x0300 >> 0 ),
        ( uint8_t ) signal_type,
        ( uint8_t ) ( channels >> 8 ),
        ( uint8_t ) ( channels >> 0 ),
        ( uint8_t ) scan_mode,
        max_results,
        nb_scan_per_channel,
        ( uint8_t ) ( timeout_in_ms >> 8 ),
        ( uint8_t ) ( timeout_in_ms >> 0 ),
        ( uint8_t ) ( ( abort_on_timeout == true ) ? 1 : 0 ),
    };

    if ( lr1110_spi_write( context, cbuffer, 2+9 ) == LR1110_SPI_STATUS_OK ) {
        return LR11XX_STATUS_OK1;
    } else {
        return LR11XX_STATUS_ERROR1;
    }
}

static uint8_t lr11xx_wifi_get_result_size_from_format( const lr11xx_wifi_result_format_t1 format ) {
    uint8_t result_size = 0;
    switch( format ) {
      case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE: {
          result_size = 22;
          break;
      }
      case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL: {
          result_size = 9;
          break;
      }
      case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL: {
          result_size = 79;
          break;
      }
    }
    return result_size;
}

static uint8_t lr11xx_wifi_get_format_code( const lr11xx_wifi_result_format_t1 format ) {
    uint8_t format_code = 0x00;
    switch( format ) {
      case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE: {
          format_code = 0x01;
          break;
      }
      case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL: {
          format_code = 0x04;
          break;
      }
      case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL: {
          format_code = 0x01;
          break;
      }
    }
    return format_code;
}

static lr11xx_hal_status_t1 lr11xx_wifi_read_results_helper( const void* context, const uint8_t start_index,
                                                            const uint8_t n_elem, uint8_t* buffer,
                                                            const lr11xx_wifi_result_format_t1 result_format ) {
    const uint8_t  size_single_elem   = lr11xx_wifi_get_result_size_from_format( result_format );
    const uint8_t  result_format_code = lr11xx_wifi_get_format_code( result_format );
    const uint8_t  cbuffer[2+3] = { ( uint8_t ) ( 0x0306 >> 8 ), ( uint8_t ) ( 0x0306 & 0x00FF ), start_index, n_elem, result_format_code };
    const uint16_t size_total                                  = n_elem * size_single_elem;
    //return (lr11xx_hal_status_t1) lr11xx_hal_read( context, cbuffer, 2+3, buffer, size_total );
    return (lr11xx_hal_status_t1) lr1110_spi_read( context, cbuffer, 2+3, buffer, size_total );
}

static void lr11xx_wifi_read_mac_address_from_buffer( const uint8_t* buffer, const uint16_t index_in_buffer,
                                                      lr11xx_wifi_mac_address_t1 mac_address ) {
    for( uint8_t field_mac_index = 0; field_mac_index < 6; field_mac_index++ ) {
        mac_address[field_mac_index] = buffer[index_in_buffer + field_mac_index];
    }
}

static uint16_t uint16_from_array( const uint8_t* array, const uint16_t index ) {
    return ( uint16_t ) ( array[index] << 8 ) + ( ( uint16_t ) ( array[index + 1] ) );
}

static uint64_t uint64_from_array( const uint8_t* array, const uint16_t index ) {
    return ( ( uint64_t ) ( array[index] ) << 56 ) + ( ( uint64_t ) ( array[index + 1] ) << 48 ) +
           ( ( uint64_t ) ( array[index + 2] ) << 40 ) + ( ( uint64_t ) ( array[index + 3] ) << 32 ) +
           ( ( uint64_t ) ( array[index + 4] ) << 24 ) + ( ( uint64_t ) ( array[index + 5] ) << 16 ) +
           ( ( uint64_t ) ( array[index + 6] ) << 8 ) + ( uint64_t ) ( array[index + 7] );
}

static void interpret_basic_complete_result_from_buffer( const uint8_t  nb_results,
                                                         const uint8_t  index_result_start_writing,
                                                         const uint8_t* buffer,
                                                         lr11xx_wifi_basic_complete_result_t1* result ) {
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ ) {
        const uint16_t                       local_index_start = 22 * result_index;
        lr11xx_wifi_basic_complete_result_t1* local_wifi_result = &result[index_result_start_writing + result_index];
        local_wifi_result->data_rate_info_byte                 = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte                   = buffer[local_index_start + 1];
        local_wifi_result->rssi                                = buffer[local_index_start + 2];
        local_wifi_result->frame_type_info_byte                = buffer[local_index_start + 3];
        lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 4, local_wifi_result->mac_address );
        local_wifi_result->phi_offset       = uint16_from_array( buffer, local_index_start + 10 );
        local_wifi_result->timestamp_us     = uint64_from_array( buffer, local_index_start + 12 );
        local_wifi_result->beacon_period_tu = uint16_from_array( buffer, local_index_start + 20 );
    }
}

static void interpret_basic_mac_type_channel_result_from_buffer( const uint8_t  nb_results,
                                                                 const uint8_t  index_result_start_writing,
                                                                 const uint8_t* buffer,
                                                                 lr11xx_wifi_basic_mac_type_channel_result_t1* result ) {
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ ) {
        const uint16_t local_index_start = 9 * result_index;
        lr11xx_wifi_basic_mac_type_channel_result_t1* local_wifi_result =
            &result[index_result_start_writing + result_index];
        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 3, local_wifi_result->mac_address );
    }
}

void interpret_extended_full_result_from_buffer( const uint8_t nb_results, const uint8_t index_result_start_writing,
                                                 const uint8_t* buffer, lr11xx_wifi_extended_full_result_t1* result ) {
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ ) {
        const uint16_t local_index_start = 79 * result_index;
        lr11xx_wifi_extended_full_result_t1* local_wifi_result = &result[index_result_start_writing + result_index];

        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        local_wifi_result->rate                = buffer[local_index_start + 3];
        local_wifi_result->service             = uint16_from_array( buffer, local_index_start + 4 );
        local_wifi_result->length              = uint16_from_array( buffer, local_index_start + 6 );
        local_wifi_result->frame_control       = uint16_from_array( buffer, local_index_start + 8 );
        lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 10, local_wifi_result->mac_address_1 );
        lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 16, local_wifi_result->mac_address_2 );
        lr11xx_wifi_read_mac_address_from_buffer( buffer, local_index_start + 22, local_wifi_result->mac_address_3 );
        local_wifi_result->timestamp_us     = uint64_from_array( buffer, local_index_start + 28 );
        local_wifi_result->beacon_period_tu = uint16_from_array( buffer, local_index_start + 36 );
        local_wifi_result->seq_control      = uint16_from_array( buffer, local_index_start + 38 );
        for( uint8_t ssid_index = 0; ssid_index < 32; ssid_index++ ) {
            local_wifi_result->ssid_bytes[ssid_index] = buffer[local_index_start + ssid_index + 40];
        }
        local_wifi_result->current_channel               = buffer[local_index_start + 72];
        local_wifi_result->country_code[0]               = buffer[local_index_start + 73];
        local_wifi_result->country_code[1]               = buffer[local_index_start + 74];
        local_wifi_result->io_regulation                 = buffer[local_index_start + 75];
        local_wifi_result->fcs_check_byte.is_fcs_checked = ( ( buffer[local_index_start + 76] & 0x01 ) == 0x01 );
        local_wifi_result->fcs_check_byte.is_fcs_ok      = ( ( buffer[local_index_start + 76] & 0x02 ) == 0x02 );
        local_wifi_result->phi_offset                    = uint16_from_array( buffer, local_index_start + 77 );
    }
}

static void generic_results_interpreter( const uint8_t n_result_to_parse, const uint8_t index_result_start_writing,
                                         const uint8_t* buffer, lr11xx_wifi_result_interface_t1 result_interface,
                                         const lr11xx_wifi_result_format_t1 format_code ) {
    switch( format_code ) {
      case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE: {
          interpret_basic_complete_result_from_buffer( n_result_to_parse, index_result_start_writing, buffer,
                                                      result_interface.basic_complete );
          break;
      }

      case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL: {
          interpret_basic_mac_type_channel_result_from_buffer( n_result_to_parse, index_result_start_writing, buffer,
                                                              result_interface.basic_mac_type_channel );
          break;
      }

      case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL: {
          interpret_extended_full_result_from_buffer( n_result_to_parse, index_result_start_writing, buffer,
                                                      result_interface.extended_complete );
          break;
      }
    }
}

uint8_t min( const uint8_t a, const uint8_t b ) {
    return ( a < b ) ? a : b;
}

static lr11xx_status_t1 fetch_and_aggregate_all_results( const void* context, const uint8_t index_result_start,
                                                        const uint8_t                     nb_results,
                                                        const uint8_t                     nb_results_per_chunk_max,
                                                        const lr11xx_wifi_result_format_t1 result_format_code,
                                                        uint8_t*                          result_buffer,
                                                        lr11xx_wifi_result_interface_t1    result_structures ) {
    uint8_t index_to_read              = index_result_start;
    uint8_t index_result_start_writing = 0;
    uint8_t remaining_results          = nb_results;

    lr11xx_hal_status_t1 hal_status = LR11XX_HAL_STATUS_OK1;
    while( remaining_results > 0 ) {
        uint8_t results_to_read = min( remaining_results, nb_results_per_chunk_max );

        lr11xx_hal_status_t1 local_hal_status = lr11xx_wifi_read_results_helper( context, index_to_read, results_to_read,
                                                                                result_buffer, result_format_code );
        if( local_hal_status != LR11XX_HAL_STATUS_OK1 ) {
            return ( lr11xx_status_t1 ) local_hal_status;
        }

        generic_results_interpreter( results_to_read, index_result_start_writing, result_buffer, result_structures,
                                     result_format_code ); {
        // Reset the content of the result_buffer in case there are still results to fetch
            const uint16_t result_buffer_size = lr11xx_wifi_get_result_size_from_format( result_format_code ) * ( min( ( 1020 ) / ( lr11xx_wifi_get_result_size_from_format( result_format_code ) ), 32 ) );
            for( uint16_t index = 0; index < result_buffer_size; index++ ) {
                result_buffer[index] = 0;
            }
        }

        index_to_read += results_to_read;
        index_result_start_writing += results_to_read;
        remaining_results -= results_to_read;
    }
    return ( lr11xx_status_t1 ) hal_status;
}

lr11xx_status_t1 lr11xx_wifi_read_extended_full_results( const void* context, const uint8_t start_result_index,
                                                        const uint8_t                       nb_results,
                                                        lr11xx_wifi_extended_full_result_t1* results ) {
    uint8_t result_buffer1[1020] = { 0 };      // uint8_t result_buffer1[79 * ( MIN( ( 1020 ) / ( 79 ), 32 ) )] = { 0 };
    const uint8_t nb_results_per_chunk_max = 12;  // const uint8_t nb_results_per_chunk_max = MIN( ( 1020 ) / ( 79 ), 32 );

    lr11xx_wifi_result_interface_t1 result_interface = { 0 };
    result_interface.extended_complete              = results;

    return fetch_and_aggregate_all_results( context, start_result_index, nb_results, nb_results_per_chunk_max,
                                            LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL, result_buffer1, result_interface );
}

lr11xx_status_t1 lr11xx_wifi_get_nb_results( const void* context, uint8_t* nb_results  ) {
    const uint8_t cbuffer[2] = { ( uint8_t ) ( 0x0305 >> 8 ), ( uint8_t ) ( 0x0305 >> 0 ) };
    uint8_t       rbuffer[2] = { 0 };

    if ( lr1110_spi_read( context, cbuffer, 2, rbuffer, 2 ) == LR1110_SPI_STATUS_OK ) {
        *nb_results = rbuffer[1];
        return LR11XX_STATUS_OK1;
    } else {
        return LR11XX_STATUS_ERROR1;
    }
}

void scanWiFiNetworks( const void* context ) {
    HAL_DBG_TRACE_PRINTF( "\r\n-----------------WIFI Scan-----------------\r\n" );

    if (lr11xx_wifi_scan( context, LR11XX_WIFI_TYPE_SCAN_B_G_N, 0x3FFF, LR11XX_WIFI_SCAN_MODE_UNTIL_SSID, 12, 3, 110, true ) != LR11XX_STATUS_OK1) {
        HAL_DBG_TRACE_ERROR( "Failed to scan Wi-Fi networks\r\n" );
        return;
    }

    HAL_Delay( 500 );

    uint8_t nb_results;
    if (lr11xx_wifi_get_nb_results( context, &nb_results ) != LR11XX_STATUS_OK1) {
        HAL_DBG_TRACE_INFO( "Failed to get Wi-Fi networks count\r\n" );
        return;
    }

    HAL_DBG_TRACE_PRINTF( "Number of Wi-Fi networks found: %d\r\n", nb_results );
    
    HAL_Delay( 500 );

    lr11xx_wifi_extended_full_result_t1 wifi_scan_result;
    for( int i = 0; i < nb_results; i++ ) {
        if (lr11xx_wifi_read_extended_full_results( context, i, 1, &wifi_scan_result ) != LR11XX_STATUS_OK1) {
            HAL_DBG_TRACE_ERROR( "Failed to read Wi-Fi networks\r\n" );
            return;
        }
        HAL_DBG_TRACE_PRINTF( "wifi %d: SSID: %s, ", i, wifi_scan_result.ssid_bytes );
        HAL_DBG_TRACE_PRINTF( "mac: %02x:%02x:%02x:%02x:%02x:%02x, rssi: %d\r\n", wifi_scan_result.mac_address_3[0], wifi_scan_result.mac_address_3[1], wifi_scan_result.mac_address_3[2], wifi_scan_result.mac_address_3[3], wifi_scan_result.mac_address_3[4], wifi_scan_result.mac_address_3[5], wifi_scan_result.rssi );
    }
}