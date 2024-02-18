

#include <stdint.h>
#include <stdbool.h>



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

typedef struct lr11xx_wifi_basic_mac_type_channel_result_s1
{
    lr11xx_wifi_datarate_info_byte_t1 data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1  channel_info_byte;
    int8_t                           rssi;
    lr11xx_wifi_mac_address_t1        mac_address;
} lr11xx_wifi_basic_mac_type_channel_result_t1;

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

typedef uint8_t lr11xx_wifi_country_code_str_t1[2];

typedef struct lr11xx_wifi_fcs_info_byte_s1
{
    bool is_fcs_ok;       //!< True if the LR11XX has checked the FCS and the check succeeded
    bool is_fcs_checked;  //!< True if the LR11XX has checked the FCS
} lr11xx_wifi_fcs_info_byte_t1;

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

typedef union
{
    lr11xx_wifi_basic_complete_result_t1*         basic_complete;
    lr11xx_wifi_basic_mac_type_channel_result_t1* basic_mac_type_channel;
    lr11xx_wifi_extended_full_result_t1*          extended_complete;
} lr11xx_wifi_result_interface_t1;

typedef enum
{
    LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE,          //!< Basic complete result format: @ref
                                                       //!< lr11xx_wifi_basic_complete_result_t
    LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL,  //!<  Basic MAC/type/channel result format: @ref
                                                       //!<  lr11xx_wifi_basic_mac_type_channel_result_t
    LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL,  //!< Extended full result format: @ref lr11xx_wifi_extended_full_result_t
} lr11xx_wifi_result_format_t1;

typedef enum lr11xx_hal_status_e1
{
    LR11XX_HAL_STATUS_OK1    = 0,
    LR11XX_HAL_STATUS_ERROR1 = 3,
} lr11xx_hal_status_t1;

typedef enum
{
    LR11XX_WIFI_TYPE_SCAN_B     = 0x01,  //!< Wi-Fi B
    LR11XX_WIFI_TYPE_SCAN_G     = 0x02,  //!< Wi-Fi G
    LR11XX_WIFI_TYPE_SCAN_N     = 0x03,  //!< Wi-Fi N
    LR11XX_WIFI_TYPE_SCAN_B_G_N = 0x04,  //!< Wi-Fi B and Wi-Fi G/N
} lr11xx_wifi_signal_type_scan_t1;

typedef uint16_t lr11xx_wifi_channel_mask_t1;

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

typedef enum lr11xx_status_e1
{
    LR11XX_STATUS_OK    = 0,
    LR11XX_STATUS_ERROR = 3,
} lr11xx_status_t1;



void scanWiFiNetworks( const void* context );

/*!
 * @brief Get LR1110 Wi-Fi version
 *
 * @param [in] context Radio abstraction
 */
void getWiFi_Version( const void* context);