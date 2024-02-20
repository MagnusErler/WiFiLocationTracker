#include <stdint.h>
#include <stdbool.h>


// LENGTHS FOR COMMANDS
#define LR1110_CMD_NO_PARAM_LENGTH                  2
#define LR1110_GET_WIFI_VERSION_CMD_LENGTH          LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_NB_RESULTS_WIFI_CMD_LENGTH       LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_SCAN_WIFI_CMD_LENGTH                 LR1110_CMD_NO_PARAM_LENGTH + 9

// LENGTHS FOR RESPONSES
#define LR1110_NO_PARAM_LENGTH                      2
#define LR1110_GET_WIFI_VERSION_LENGTH              LR1110_NO_PARAM_LENGTH
#define LR1110_GET_NB_RESULTS_WIFI_LENGTH           1

// LR1110 WIFI COMMANDS
#define LR1110_WIFI_GET_FIRMWARE_WIFI_VERSION_CMD   0x0320
#define LR1110_WIFI_GET_NUMBER_OF_RESULTS_CMD       0x0305
#define LR1110_WIFI_SCAN_WIFI_NETWORKS_CMD          0x0300

/*!
 * @brief Type to store a WiFi datarate info byte
 */
typedef uint8_t lr11xx_wifi_datarate_info_byte_t;

/*!
 * @brief Type to store a WiFi channel info byte
 */
typedef uint8_t lr11xx_wifi_channel_info_byte_t;

/*!
 * @brief Type to store a WiFi frame type info byte
 */
typedef uint8_t lr11xx_wifi_frame_type_info_byte_t;

/*!
 * @brief Type to store a MAC address
 */
typedef uint8_t lr11xx_wifi_mac_address_t[6];

typedef struct lr11xx_wifi_basic_complete_result_s
{
    lr11xx_wifi_datarate_info_byte_t   data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t    channel_info_byte;
    int8_t                             rssi;
    lr11xx_wifi_frame_type_info_byte_t frame_type_info_byte;
    lr11xx_wifi_mac_address_t          mac_address;
    int16_t                            phi_offset;
    uint64_t timestamp_us;  //!< Indicate the up-time of the Access Point transmitting the Beacon [us]
    uint16_t beacon_period_tu;
} lr11xx_wifi_basic_complete_result_t;

typedef struct lr11xx_wifi_basic_mac_type_channel_result_s
{
    lr11xx_wifi_datarate_info_byte_t data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t  channel_info_byte;
    int8_t                           rssi;
    lr11xx_wifi_mac_address_t        mac_address;
} lr11xx_wifi_basic_mac_type_channel_result_t;

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
} lr11xx_wifi_channel_t;

typedef uint8_t lr11xx_wifi_country_code_str_t[2];

typedef struct lr11xx_wifi_fcs_info_byte_s
{
    bool is_fcs_ok;       //!< True if the LR11XX has checked the FCS and the check succeeded
    bool is_fcs_checked;  //!< True if the LR11XX has checked the FCS
} lr11xx_wifi_fcs_info_byte_t;

typedef struct
{
    lr11xx_wifi_datarate_info_byte_t data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t  channel_info_byte;
    int8_t                           rssi;
    uint8_t                          rate;           //!< Rate index
    uint16_t                         service;        //!< Service value
    uint16_t                         length;         //!< Length of MPDU (in microseconds for WiFi B, bytes for WiFi G)
    uint16_t                         frame_control;  //!< Frame Control structure
    lr11xx_wifi_mac_address_t        mac_address_1;
    lr11xx_wifi_mac_address_t        mac_address_2;
    lr11xx_wifi_mac_address_t        mac_address_3;
    uint64_t                         timestamp_us;  //!< Indicate the up-time of the Access Point
                                                    //!< transmitting the Beacon [us]
    uint16_t beacon_period_tu;
    uint16_t seq_control;                                 //!< Sequence Control value
    uint8_t  ssid_bytes[32];  //!< Service Set
                                                          //!< IDentifier
    lr11xx_wifi_channel_t          current_channel;       //!< Current channel indicated in the WiFi frame
    lr11xx_wifi_country_code_str_t country_code;          //!< Country Code
    uint8_t                        io_regulation;         //!< Input Output Regulation
    lr11xx_wifi_fcs_info_byte_t    fcs_check_byte;        //!< Frame Check Sequence info
    int16_t                        phi_offset;
} lr11xx_wifi_extended_full_result_t;

typedef union
{
    lr11xx_wifi_basic_complete_result_t*         basic_complete;
    lr11xx_wifi_basic_mac_type_channel_result_t* basic_mac_type_channel;
    lr11xx_wifi_extended_full_result_t*          extended_complete;
} lr11xx_wifi_result_interface_t;

typedef enum
{
    LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE,          //!< Basic complete result format: @ref
                                                       //!< lr11xx_wifi_basic_complete_result_t
    LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL,  //!<  Basic MAC/type/channel result format: @ref
                                                       //!<  lr11xx_wifi_basic_mac_type_channel_result_t
    LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL,  //!< Extended full result format: @ref lr11xx_wifi_extended_full_result_t
} lr11xx_wifi_result_format_t;

typedef uint16_t lr11xx_wifi_channel_mask_t;

/*!
 * @brief Status code of WIFI operations
 */
typedef enum lr1110_wifi_status_e {
    LR1110_WIFI_STATUS_OK        = 0x00,  //!< WIFI operation executed successfuly
    LR1110_WIFI_STATUS_ERROR     = 0xFF,  //!< WIFI operation failed
} lr1110_wifi_status_t;

typedef enum {
    LR11XX_WIFI_TYPE_SCAN_B     = 0x01,  //!< WiFi B
    LR11XX_WIFI_TYPE_SCAN_G     = 0x02,  //!< WiFi G
    LR11XX_WIFI_TYPE_SCAN_N     = 0x03,  //!< WiFi N
    LR11XX_WIFI_TYPE_SCAN_B_G_N = 0x04,  //!< WiFi B and WiFi G/N
} lr11xx_wifi_signal_type_scan_t;

typedef enum {
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
} lr11xx_wifi_mode_t;

/*!
 * @brief Scan WiFi networks
 *
 * @param [in] context Radio abstraction
 * @param [in] signal_type Type of signal to scan
 * @param [in] chan_mask Mask of channels to scan
 * @param [in] acq_mode Acquisition mode
 * @param [in] nb_max_res Maximum number of results to return
 * @param [in] nb_scan_per_chan Number of scan per channel
 * @param [in] timeout Timeout for the scan
 * @param [in] abort_on_timeout If true, the scan will be aborted on timeout
 * 
 * @return Operation status
 */
lr1110_wifi_status_t scanLR1110_WiFi_Networks( const void* context, const lr11xx_wifi_signal_type_scan_t signal_type, 
                                    const lr11xx_wifi_channel_mask_t chan_mask, const lr11xx_wifi_mode_t acq_mode, 
                                    const uint8_t nb_max_res, const uint8_t nb_scan_per_chan, const uint16_t timeout, const bool abort_on_timeout );

/*!
 * @brief Get LR1110 WiFi version
 *
 * @param [in] context Radio abstraction
 * 
 * @return Operation status
 */
lr1110_wifi_status_t getLR1110_WiFi_Version( const void* context);

/*!
 * @brief Get the number of WiFi results
 *
 * @param [in] context Radio abstraction
 * 
 * @return Operation status
 */
lr1110_wifi_status_t getLR1110_WiFi_Number_of_Results( const void* context );