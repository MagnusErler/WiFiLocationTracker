
#include <stdbool.h>  // bool type

#include "smtc_hal_dbg_trace.h"

#include "ralf_lr11xx.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

const ralf_t modem_radio = RALF_LR11XX_INSTANTIATE( NULL );



/*!
 * @brief Type to store a Wi-Fi datarate info byte
 */
typedef uint8_t lr11xx_wifi_datarate_info_byte_t1;

/*!
 * @brief Type to store a Wi-Fi channel info byte
 */
typedef uint8_t lr11xx_wifi_channel_info_byte_t1;

/*!
 * @brief Type to store a Wi-Fi frame type info byte
 */
typedef uint8_t lr11xx_wifi_frame_type_info_byte_t1;

/*!
 * @brief Type to store a MAC address
 */
typedef uint8_t lr11xx_wifi_mac_address_t1[6];

/*!
 * @brief Basic complete result structure
 *
 * The beacon period is expressed in TU (Time Unit). 1 TU is 1024 microseconds.
 */
typedef struct lr11xx_wifi_basic_complete_result_s1
{
    lr11xx_wifi_datarate_info_byte_t1   data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1    channel_info_byte;
    int8_t                             rssi;
    lr11xx_wifi_frame_type_info_byte_t1 frame_type_info_byte;
    lr11xx_wifi_mac_address_t1          mac_address;
    int16_t                            phi_offset;
    uint64_t timestamp_us;  //!< Indicate the up-time of the Access Point transmitting the Beacon [us]
    uint16_t beacon_period_tu;
} lr11xx_wifi_basic_complete_result_t1;

/*!
 * @brief Basic MAC, type, channel result structure
 */
typedef struct lr11xx_wifi_basic_mac_type_channel_result_s1
{
    lr11xx_wifi_datarate_info_byte_t1 data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1  channel_info_byte;
    int8_t                           rssi;
    lr11xx_wifi_mac_address_t1        mac_address;
} lr11xx_wifi_basic_mac_type_channel_result_t1;

/*!
 * @brief Wi-Fi Channels index
 */
typedef enum
{
    LR11XX_WIFI_NO_CHANNEL   = 0x00,
    LR11XX_WIFI_CHANNEL_1    = 0x01,  //!< Channel at frequency 2.412 GHz
    LR11XX_WIFI_CHANNEL_2    = 0x02,  //!< Channel at frequency 2.417 GHz
    LR11XX_WIFI_CHANNEL_3    = 0x03,  //!< Channel at frequency 2.422 GHz
    LR11XX_WIFI_CHANNEL_4    = 0x04,  //!< Channel at frequency 2.427 GHz
    LR11XX_WIFI_CHANNEL_5    = 0x05,  //!< Channel at frequency 2.432 GHz
    LR11XX_WIFI_CHANNEL_6    = 0x06,  //!< Channel at frequency 2.437 GHz
    LR11XX_WIFI_CHANNEL_7    = 0x07,  //!< Channel at frequency 2.442 GHz
    LR11XX_WIFI_CHANNEL_8    = 0x08,  //!< Channel at frequency 2.447 GHz
    LR11XX_WIFI_CHANNEL_9    = 0x09,  //!< Channel at frequency 2.452 GHz
    LR11XX_WIFI_CHANNEL_10   = 0x0A,  //!< Channel at frequency 2.457 GHz
    LR11XX_WIFI_CHANNEL_11   = 0x0B,  //!< Channel at frequency 2.462 GHz
    LR11XX_WIFI_CHANNEL_12   = 0x0C,  //!< Channel at frequency 2.467 GHz
    LR11XX_WIFI_CHANNEL_13   = 0x0D,  //!< Channel at frequency 2.472 GHz
    LR11XX_WIFI_CHANNEL_14   = 0x0E,  //!< Channel at frequency 2.484 GHz
    LR11XX_WIFI_ALL_CHANNELS = 0x0F,
} lr11xx_wifi_channel_t1;

/*!
 * @brief Type to store the Country Code
 */
typedef uint8_t lr11xx_wifi_country_code_str_t1[2];

/*!
 * @brief Wi-Fi FCS info byte
 */
typedef struct lr11xx_wifi_fcs_info_byte_s1
{
    bool is_fcs_ok;       //!< True if the LR11XX has checked the FCS and the check succeeded
    bool is_fcs_checked;  //!< True if the LR11XX has checked the FCS
} lr11xx_wifi_fcs_info_byte_t1;

/*!
 * @brief Extended full result structure
 *
 * @note The beacon period is expressed in TU (Time Unit). 1 TU is 1024 microseconds.
 *
 * @remark When used with @ref ::LR11XX_WIFI_SCAN_MODE_UNTIL_SSID, the following field are always set to 0:
 *      - field is_fcs_ok and is_fcs_checked in fcs_check_byte structure
 *      - current_channel
 *      - country_code
 *      - io_regulation
 */
typedef struct
{
    lr11xx_wifi_datarate_info_byte_t1 data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1  channel_info_byte;
    int8_t                           rssi;
    uint8_t                          rate;           //!< Rate index
    uint16_t                         service;        //!< Service value
    uint16_t                         length;         //!< Length of MPDU (in microseconds for WiFi B, bytes for WiFi G)
    uint16_t                         frame_control;  //!< Frame Control structure
    lr11xx_wifi_mac_address_t1        mac_address_1;
    lr11xx_wifi_mac_address_t1        mac_address_2;
    lr11xx_wifi_mac_address_t1        mac_address_3;
    uint64_t                         timestamp_us;  //!< Indicate the up-time of the Access Point
                                                    //!< transmitting the Beacon [us]
    uint16_t beacon_period_tu;
    uint16_t seq_control;                                 //!< Sequence Control value
    uint8_t  ssid_bytes[32];  //!< Service Set
                                                          //!< IDentifier
    lr11xx_wifi_channel_t1          current_channel;       //!< Current channel indicated in the Wi-Fi frame
    lr11xx_wifi_country_code_str_t1 country_code;          //!< Country Code
    uint8_t                        io_regulation;         //!< Input Output Regulation
    lr11xx_wifi_fcs_info_byte_t1    fcs_check_byte;        //!< Frame Check Sequence info
    int16_t                        phi_offset;
} lr11xx_wifi_extended_full_result_t1;

/*!
 * @brief Wi-Fi scan results interface
 */
typedef union
{
    lr11xx_wifi_basic_complete_result_t1*         basic_complete;
    lr11xx_wifi_basic_mac_type_channel_result_t1* basic_mac_type_channel;
    lr11xx_wifi_extended_full_result_t1*          extended_complete;
} lr11xx_wifi_result_interface_t1;

/*!
 * @brief Wi-Fi scan result formats
 *
 * @note Result format to use depends on the scan mode selected when calling @ref lr11xx_wifi_scan or @ref
 * lr11xx_wifi_scan_time_limit API. Refer to @ref lr11xx_wifi_are_scan_mode_result_format_compatible to know which
 * result formats are available depending on scan mode selected.
 *
 * @see lr11xx_wifi_are_scan_mode_result_format_compatible
 */
typedef enum
{
    LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE,          //!< Basic complete result format: @ref
                                                       //!< lr11xx_wifi_basic_complete_result_t
    LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL,  //!<  Basic MAC/type/channel result format: @ref
                                                       //!<  lr11xx_wifi_basic_mac_type_channel_result_t
    LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL,  //!< Extended full result format: @ref lr11xx_wifi_extended_full_result_t
} lr11xx_wifi_result_format_t1;

/*!
 * @brief LR11XX HAL status
 */
typedef enum lr11xx_hal_status_e1
{
    LR11XX_HAL_STATUS_OK1    = 0,
    LR11XX_HAL_STATUS_ERROR1 = 3,
} lr11xx_hal_status_t1;

/*!
 * @brief Wi-Fi signal type for passive scanning configuration
 *
 * Note it is not possible to configure the WiFi passive scanning to search Wi-Fi type N GreenField. Only Wi-Fi type N
 * Mixed Mode can be scanned by LR11XX.
 *
 * @warning ::LR11XX_WIFI_TYPE_SCAN_G and ::LR11XX_WIFI_TYPE_SCAN_N configurations are implemented the same way, and
 * both will scan Wi-Fi type G **AND** Wi-Fi type N.
 */
typedef enum
{
    LR11XX_WIFI_TYPE_SCAN_B     = 0x01,  //!< Wi-Fi B
    LR11XX_WIFI_TYPE_SCAN_G     = 0x02,  //!< Wi-Fi G
    LR11XX_WIFI_TYPE_SCAN_N     = 0x03,  //!< Wi-Fi N
    LR11XX_WIFI_TYPE_SCAN_B_G_N = 0x04,  //!< Wi-Fi B and Wi-Fi G/N
} lr11xx_wifi_signal_type_scan_t1;

/*!
 * @brief Type to store a Wi-Fi channel mask
 */
typedef uint16_t lr11xx_wifi_channel_mask_t1;

/*!
 * @brief Wi-Fi scan mode
 *
 * When the LR11XX receives a Wi-Fi frame, it starts demodulating it. Depending on the scan mode selected, only some
 * Wi-Fi frame type/sub-types are to be kept. The demodulation step is stopped as soon as the LR11XX detects the current
 * Wi-Fi frame is not of the required type/sub-types. This saves scan time and consumption.
 *
 * A Wi-Fi frame is never completely demodulated. The ::LR11XX_WIFI_SCAN_MODE_FULL_BEACON uses a special configuration
 * allowing to demodulate more fields (until Frame Check Sequence field), at a price of higher scan duration and higher
 * consumption.
 *
 * @note Not all results formats are available depending on the scan mode selected. Refer to
 * @ref lr11xx_wifi_are_scan_mode_result_format_compatible to know which result formats are available depending on scan
 * mode selected.
 *
 * @see lr11xx_wifi_are_scan_mode_result_format_compatible
 */
typedef enum
{
    LR11XX_WIFI_SCAN_MODE_BEACON =
        1,  //!< Exposes Beacons and Probe Responses Access Points frames until Period Beacon field (Basic result)
    LR11XX_WIFI_SCAN_MODE_BEACON_AND_PKT =
        2,  //!< Exposes some Management Access Points frames until Period Beacon field, and some other packets frame
            //!< until third Mac Address field (Basic result)
    LR11XX_WIFI_SCAN_MODE_FULL_BEACON =
        4,  //!< Exposes Beacons and Probes Responses Access Points frames until Frame Check Sequence (FCS) field
            //!< (Extended result). In this mode, only signal type LR11XX_WIFI_TYPE_SCAN_B is executed and other signal
            //!< types are silently discarded.
    LR11XX_WIFI_SCAN_MODE_UNTIL_SSID = 5,  //!< Exposes Beacons and Probes Responses Access Points frames until the end
                                           //!< of SSID field (Extended result) - available since firmware 0x0306
} lr11xx_wifi_mode_t1;

/**
 * @brief LR11XX status
 */
typedef enum lr11xx_status_e1
{
    LR11XX_STATUS_OK1    = 0,
    LR11XX_STATUS_ERROR1 = 3,
} lr11xx_status_t1;


static void wifi_scan( void );


int main( void ) {

    hal_mcu_init( );
    SMTC_HAL_TRACE_PRINTF("_______________________________________________________________________________\r\n");

    while( 1 ) {
        wifi_scan();

        HAL_Delay(2000);
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */


lr11xx_status_t1 lr11xx_wifi_scan1( const void* context, const lr11xx_wifi_signal_type_scan_t1 signal_type,
                                  const lr11xx_wifi_channel_mask_t1 channels, const lr11xx_wifi_mode_t1 scan_mode,
                                  const uint8_t max_results, const uint8_t nb_scan_per_channel,
                                  const uint16_t timeout_in_ms, const bool abort_on_timeout )
{
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

    return ( lr11xx_status_t1 ) lr11xx_hal_write( context, cbuffer, 2+9, 0, 0 );
}

static uint8_t lr11xx_wifi_get_result_size_from_format1( const lr11xx_wifi_result_format_t1 format )
{
    uint8_t result_size = 0;
    switch( format )
    {
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE:
    {
        result_size = 22;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL:
    {
        result_size = 9;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL:
    {
        result_size = 79;
        break;
    }
    }
    return result_size;
}

static uint8_t lr11xx_wifi_get_format_code1( const lr11xx_wifi_result_format_t1 format )
{
    uint8_t format_code = 0x00;
    switch( format )
    {
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE:
    {
        format_code = 0x01;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL:
    {
        format_code = 0x04;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL:
    {
        format_code = 0x01;
        break;
    }
    }
    return format_code;
}

static lr11xx_hal_status_t1 lr11xx_wifi_read_results_helper1( const void* context, const uint8_t start_index,
                                                            const uint8_t n_elem, uint8_t* buffer,
                                                            const lr11xx_wifi_result_format_t1 result_format )
{
    const uint8_t  size_single_elem   = lr11xx_wifi_get_result_size_from_format1( result_format );
    const uint8_t  result_format_code = lr11xx_wifi_get_format_code1( result_format );
    const uint8_t  cbuffer[2+3] = { ( uint8_t ) ( 0x0306 >> 8 ),
                                                                  ( uint8_t ) ( 0x0306 & 0x00FF ),
                                                                  start_index, n_elem, result_format_code };
    const uint16_t size_total                                  = n_elem * size_single_elem;
    return (lr11xx_hal_status_t1) lr11xx_hal_read( context, cbuffer, 2+3, buffer, size_total );
}

static void lr11xx_wifi_read_mac_address_from_buffer1( const uint8_t* buffer, const uint16_t index_in_buffer,
                                                      lr11xx_wifi_mac_address_t1 mac_address )
{
    for( uint8_t field_mac_index = 0; field_mac_index < 6; field_mac_index++ )
    {
        mac_address[field_mac_index] = buffer[index_in_buffer + field_mac_index];
    }
}

static uint16_t uint16_from_array1( const uint8_t* array, const uint16_t index )
{
    return ( uint16_t ) ( array[index] << 8 ) + ( ( uint16_t ) ( array[index + 1] ) );
}

static uint64_t uint64_from_array1( const uint8_t* array, const uint16_t index )
{
    return ( ( uint64_t ) ( array[index] ) << 56 ) + ( ( uint64_t ) ( array[index + 1] ) << 48 ) +
           ( ( uint64_t ) ( array[index + 2] ) << 40 ) + ( ( uint64_t ) ( array[index + 3] ) << 32 ) +
           ( ( uint64_t ) ( array[index + 4] ) << 24 ) + ( ( uint64_t ) ( array[index + 5] ) << 16 ) +
           ( ( uint64_t ) ( array[index + 6] ) << 8 ) + ( uint64_t ) ( array[index + 7] );
}

static void interpret_basic_complete_result_from_buffer1( const uint8_t  nb_results,
                                                         const uint8_t  index_result_start_writing,
                                                         const uint8_t* buffer,
                                                         lr11xx_wifi_basic_complete_result_t1* result )
{
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ )
    {
        const uint16_t                       local_index_start = 22 * result_index;
        lr11xx_wifi_basic_complete_result_t1* local_wifi_result = &result[index_result_start_writing + result_index];
        local_wifi_result->data_rate_info_byte                 = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte                   = buffer[local_index_start + 1];
        local_wifi_result->rssi                                = buffer[local_index_start + 2];
        local_wifi_result->frame_type_info_byte                = buffer[local_index_start + 3];
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 4, local_wifi_result->mac_address );
        local_wifi_result->phi_offset       = uint16_from_array1( buffer, local_index_start + 10 );
        local_wifi_result->timestamp_us     = uint64_from_array1( buffer, local_index_start + 12 );
        local_wifi_result->beacon_period_tu = uint16_from_array1( buffer, local_index_start + 20 );
    }
}

static void interpret_basic_mac_type_channel_result_from_buffer1( const uint8_t  nb_results,
                                                                 const uint8_t  index_result_start_writing,
                                                                 const uint8_t* buffer,
                                                                 lr11xx_wifi_basic_mac_type_channel_result_t1* result )
{
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ )
    {
        const uint16_t local_index_start = 9 * result_index;
        lr11xx_wifi_basic_mac_type_channel_result_t1* local_wifi_result =
            &result[index_result_start_writing + result_index];
        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 3, local_wifi_result->mac_address );
    }
}

void interpret_extended_full_result_from_buffer1( const uint8_t nb_results, const uint8_t index_result_start_writing,
                                                 const uint8_t* buffer, lr11xx_wifi_extended_full_result_t1* result )
{
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ )
    {
        const uint16_t local_index_start = 79 * result_index;
        lr11xx_wifi_extended_full_result_t1* local_wifi_result = &result[index_result_start_writing + result_index];

        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        local_wifi_result->rate                = buffer[local_index_start + 3];
        local_wifi_result->service             = uint16_from_array1( buffer, local_index_start + 4 );
        local_wifi_result->length              = uint16_from_array1( buffer, local_index_start + 6 );
        local_wifi_result->frame_control       = uint16_from_array1( buffer, local_index_start + 8 );
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 10, local_wifi_result->mac_address_1 );
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 16, local_wifi_result->mac_address_2 );
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 22, local_wifi_result->mac_address_3 );
        local_wifi_result->timestamp_us     = uint64_from_array1( buffer, local_index_start + 28 );
        local_wifi_result->beacon_period_tu = uint16_from_array1( buffer, local_index_start + 36 );
        local_wifi_result->seq_control      = uint16_from_array1( buffer, local_index_start + 38 );
        for( uint8_t ssid_index = 0; ssid_index < 32; ssid_index++ )
        {
            local_wifi_result->ssid_bytes[ssid_index] = buffer[local_index_start + ssid_index + 40];
        }
        local_wifi_result->current_channel               = buffer[local_index_start + 72];
        local_wifi_result->country_code[0]               = buffer[local_index_start + 73];
        local_wifi_result->country_code[1]               = buffer[local_index_start + 74];
        local_wifi_result->io_regulation                 = buffer[local_index_start + 75];
        local_wifi_result->fcs_check_byte.is_fcs_checked = ( ( buffer[local_index_start + 76] & 0x01 ) == 0x01 );
        local_wifi_result->fcs_check_byte.is_fcs_ok      = ( ( buffer[local_index_start + 76] & 0x02 ) == 0x02 );
        local_wifi_result->phi_offset                    = uint16_from_array1( buffer, local_index_start + 77 );
    }
}

static void generic_results_interpreter1( const uint8_t n_result_to_parse, const uint8_t index_result_start_writing,
                                         const uint8_t* buffer, lr11xx_wifi_result_interface_t1 result_interface,
                                         const lr11xx_wifi_result_format_t1 format_code )
{
    switch( format_code )
    {
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE:
    {
        interpret_basic_complete_result_from_buffer1( n_result_to_parse, index_result_start_writing, buffer,
                                                     result_interface.basic_complete );
        break;
    }

    case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL:
    {
        interpret_basic_mac_type_channel_result_from_buffer1( n_result_to_parse, index_result_start_writing, buffer,
                                                             result_interface.basic_mac_type_channel );
        break;
    }

    case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL:
    {
        interpret_extended_full_result_from_buffer1( n_result_to_parse, index_result_start_writing, buffer,
                                                    result_interface.extended_complete );
        break;
    }
    }
}

uint8_t min( const uint8_t a, const uint8_t b )
{
    return ( a < b ) ? a : b;
}

static lr11xx_status_t1 fetch_and_aggregate_all_results1( const void* context, const uint8_t index_result_start,
                                                        const uint8_t                     nb_results,
                                                        const uint8_t                     nb_results_per_chunk_max,
                                                        const lr11xx_wifi_result_format_t1 result_format_code,
                                                        uint8_t*                          result_buffer,
                                                        lr11xx_wifi_result_interface_t1    result_structures )
{
    uint8_t index_to_read              = index_result_start;
    uint8_t index_result_start_writing = 0;
    uint8_t remaining_results          = nb_results;

    lr11xx_hal_status_t1 hal_status = LR11XX_HAL_STATUS_OK1;
    while( remaining_results > 0 )
    {
        uint8_t results_to_read = min( remaining_results, nb_results_per_chunk_max );

        lr11xx_hal_status_t1 local_hal_status = lr11xx_wifi_read_results_helper1( context, index_to_read, results_to_read,
                                                                                result_buffer, result_format_code );
        if( local_hal_status != LR11XX_HAL_STATUS_OK1 )
        {
            return ( lr11xx_status_t1 ) local_hal_status;
        }

        generic_results_interpreter1( results_to_read, index_result_start_writing, result_buffer, result_structures,
                                     result_format_code );

        // Reset the content of the result_buffer in case there are still results to fetch
        {
            const uint16_t result_buffer_size = lr11xx_wifi_get_result_size_from_format1( result_format_code ) * ( min( ( 1020 ) / ( lr11xx_wifi_get_result_size_from_format1( result_format_code ) ), 32 ) );
            for( uint16_t index = 0; index < result_buffer_size; index++ )
            {
                result_buffer[index] = 0;
            }
        }

        index_to_read += results_to_read;
        index_result_start_writing += results_to_read;
        remaining_results -= results_to_read;
    }
    return ( lr11xx_status_t1 ) hal_status;
}

lr11xx_status_t1 lr11xx_wifi_read_extended_full_results1( const void* radio, const uint8_t start_result_index,
                                                        const uint8_t                       nb_results,
                                                        lr11xx_wifi_extended_full_result_t1* results )
{
    uint8_t result_buffer1[1020] = { 0 };      // uint8_t result_buffer1[79 * ( MIN( ( 1020 ) / ( 79 ), 32 ) )] = { 0 };
    const uint8_t nb_results_per_chunk_max = 12;  // const uint8_t nb_results_per_chunk_max = MIN( ( 1020 ) / ( 79 ), 32 );

    lr11xx_wifi_result_interface_t1 result_interface = { 0 };
    result_interface.extended_complete              = results;

    return fetch_and_aggregate_all_results1( radio, start_result_index, nb_results, nb_results_per_chunk_max,
                                            LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL, result_buffer1, result_interface );
}

static void wifi_scan( void ) {
    SMTC_HAL_TRACE_PRINTF( "\r\n-----------------WIFI Scan-----------------\r\n" );

    lr11xx_wifi_extended_full_result_t1 wifi_scan_result;
    lr11xx_wifi_scan1( modem_radio.ral.context, LR11XX_WIFI_TYPE_SCAN_B_G_N, 0x3FFF, LR11XX_WIFI_SCAN_MODE_UNTIL_SSID, 12, 3, 110, true );
    
    HAL_Delay( 110 );

    for( int i = 0; i < 2; i++ ) {
        lr11xx_wifi_read_extended_full_results1( modem_radio.ral.context, i, 1, &wifi_scan_result );
        SMTC_HAL_TRACE_PRINTF( "wifi %d: SSID: %s, ", i, wifi_scan_result.ssid_bytes );
        SMTC_HAL_TRACE_PRINTF( "mac: %02x:%02x:%02x:%02x:%02x:%02x, rssi: %d\r\n", wifi_scan_result.mac_address_3[0], wifi_scan_result.mac_address_3[1], wifi_scan_result.mac_address_3[2], wifi_scan_result.mac_address_3[3], wifi_scan_result.mac_address_3[4], wifi_scan_result.mac_address_3[5], wifi_scan_result.rssi );
    }
}
