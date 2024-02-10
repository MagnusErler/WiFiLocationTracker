

/*!
 * @brief Length of command buffer for commands that have no parameter
 */
#define LR1110_CMD_NO_PARAM_LENGTH 2

/*!
 * @brief Length of return buffer for commands that have no parameter
 */
#define LR1110_NO_PARAM_LENGTH 2

/*!
 * @brief Length of command buffer for bootloader get version command
 */
#define LR1110_BL_VERSION_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length in byte of the LR1110 version blob
 */
#define LR1110_BL_VERSION_LENGTH LR1110_NO_PARAM_LENGTH + 2

/*!
 * @brief Length of command buffer for bootloader get temperature command
 */
#define LR1110_BL_TEMPERATURE_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length in byte of the LR1110 temperature blob
 */
#define LR1110_BL_TEMPERATURE_LENGTH LR1110_NO_PARAM_LENGTH

/*!
 * @brief Length of command buffer read GNSS firmware version
 */
#define LR1110_GNSS_VERSION_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Response buffer size for command Read firwmare version
 */
#define LR1110_GNSS_VERSION_LENGTH LR1110_NO_PARAM_LENGTH






enum {
    LR1110_BL_GET_STATUS_OC            = 0x0100,  //!< Operation code for get status command
    LR1110_BL_GET_VERSION_OC           = 0x0101,  //!< Operation code for get version command
    LR1110_BL_ERASE_FLASH_OC           = 0x8000,  //!< Operation code for flash erase command
    LR1110_BL_ERASE_PAGE_OC            = 0x8001,  //!< Operation code for flash page erase command
    LR1110_BL_WRITE_FLASH_OC           = 0x8002,  //!< Operation code for write flash command
    LR1110_BL_WRITE_FLASH_ENCRYPTED_OC = 0x8003,  //!< Operation code for encrypted flash write command
    LR1110_BL_GET_HASH_OC              = 0x8004,  //!< Operation code for hash getter command
    LR1110_BL_REBOOT_OC                = 0x8005,  //!< Operation code for reboot command
    LR1110_BL_GET_PIN_OC               = 0x800B,  //!< Operation code for PIN read command
    LR1110_BL_GET_CHIP_EUI_OC          = 0x800C,  //!< Operation code for EUI read command
    LR1110_BL_GET_TEMPERATURE          = 0x011A,  //!< Operation code for temperature read command
};

/*!
 * @brief Operation code for GNSS commands
 */
typedef enum
{
    LR1110_GNSS_SET_CONSTELLATION_CMD            = 0x00,  //!< set the constellation to use
    LR1110_GNSS_READ_CONSTELLATION_CMD           = 0x01,  //!< read the used consteallations
    LR1110_GNSS_SET_ALMANAC_UPDATE_CMD           = 0x02,  //!< set almanac update configuration
    LR1110_GNSS_READ_ALMANAC_UPDATE_CMD          = 0x03,  //!< read the almanac update configuration
    LR1110_GNSS_SET_FREQ_SEARCH_SPACE_CMD        = 0x04,  //!< set the frequency search space
    LR1110_GNSS_READ_FREQ_SEARCH_SPACE_CMD       = 0x05,  //!< read the frequency search space
    LR1110_GNSS_READ_FW_VERSION_CMD              = 0x06,  //!< read the firmware version
    LR1110_GNSS_READ_SUPPORTED_CONSTELLATION_CMD = 0x07,  //!< read the supported constellations
    LR1110_GNSS_ALMANAC_FULL_UPDATE_CMD          = 0x0E,  //!< Almanac update
    LR1110_GNSS_SET_ASSISTANCE_POSITION_CMD      = 0x10,  //!< set the assistance position
    LR1110_GNSS_READ_ASSISTANCE_POSITION_CMD     = 0x11,  //!< read the assistance position
    LR1110_GNSS_SET_XTAL_ERROR_CMD               = 0x12,  //!< set xtal accuracy
    LR1110_GNSS_READ_XTAL_ERROR_CMD              = 0x13,  //!< read the xtal accuracy
    LR1110_GNSS_PUSH_SOLVER_CMD                  = 0x14,  //!< set the informations from the solver to the chip
    LR1110_GNSS_GET_CONTEXT_STATUS_CMD           = 0x16,  //!< read the GNSS context status
    LR1110_GNSS_GET_NB_SV_DETECTED_CMD           = 0x17,  //!< read the number of detected satellites
    LR1110_GNSS_GET_SV_DETECTED_CMD              = 0x18,  //!< read the informations of detected satellites
    LR1110_GNSS_GET_TIMINGS_CMD                  = 0x19,  //!< read the meseaured timings during the scan
    LR1110_GNSS_ALMANAC_READ_BY_INDEX_CMD        = 0x1A,  //!< read the almanac by index
    LR1110_GNSS_SCAN_AUTONOMOUS_CMD              = 0x30,  //!< start the scan autonomous
    LR1110_GNSS_SCAN_ASSISTED_CMD                = 0x31,  //!< start the scan assisted
} lr1110_api_command_gnss_t;

typedef enum
{
    LR1110_GROUP_ID_SYSTEM = 0x01,  //!< Group ID for system commands
    LR1110_GROUP_ID_WIFI   = 0x03,  //!< Group ID for Wi-Fi commands
    LR1110_GROUP_ID_GNSS   = 0x04,  //!< Group ID for GNSS commands
    LR1110_GROUP_ID_MODEM  = 0x06,  //!< Group ID for modem commands
} lr1110_api_group_id_t;