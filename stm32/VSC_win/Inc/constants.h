

/*!
 * @brief Length of command buffer for commands that have no parameter
 */
#define LR1110_CMD_NO_PARAM_LENGTH 2

/*!
 * @brief Length of return buffer for commands that have no parameter
 */
#define LR1110_NO_PARAM_LENGTH 2

/*!
 * @brief Length of command buffer for LR1110 version
 */
#define LR1110_VERSION_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length of return buffer for LR1110 version
 */
#define LR1110_VERSION_LENGTH LR1110_NO_PARAM_LENGTH + 2

/*!
 * @brief Length of command buffer for LR1110 temperature
 */
#define LR1110_TEMPERATURE_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length of return buffer for LR1110 temperature
 */
#define LR1110_TEMPERATURE_LENGTH LR1110_NO_PARAM_LENGTH

/*!
 * @brief Length of command buffer for GNSS version
 */
#define LR1110_GNSS_VERSION_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length of return buffer for GNSS version
 */
#define LR1110_GNSS_VERSION_LENGTH LR1110_NO_PARAM_LENGTH

/*!
 * @brief Length of command buffer for WiFi version
 */
#define LR1110_WIFI_VERSION_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length of return buffer for WiFi version
 */
#define LR1110_WIFI_VERSION_LENGTH LR1110_NO_PARAM_LENGTH

/*!
 * @brief Length of command buffer for Chip EUI
 */
#define LR1110_CHIP_EUI_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH

/*!
 * @brief Length of return buffer for Chip EUI
 */
#define LR1110_CHIP_EUI_LENGTH LR1110_NO_PARAM_LENGTH + 6

/*!
 * @brief Length of command buffer for Set TCXO mode
 */
#define LR1110_SET_TCXO_MODE_CMD_LENGTH LR1110_CMD_NO_PARAM_LENGTH + 4


/*!
 * @brief Operation code for system commands
 */
enum
{
    LR1110_WRITE_REG_MEM_32_CMD     = 0x05,
    LR1110_READ_REG_MEM_32_CMD      = 0x06,
    LR1110_CALIBRATE_CMD            = 0x0F,
    LR1110_SET_REG_MODE_CMD         = 0x10,
    LR1110_SET_DIO_AS_RF_SWITCH_CMD = 0x12,
    LR1110_CONFIG_LF_CLOCK_CMD      = 0x16,
    LR1110_SET_TCXO_MODE_CMD        = 0x17,
    LR1110_REBOOT_CMD               = 0x18,
};

enum {
    LR1110_GET_STATUS_OC            = 0x0100,  //!< Operation code for get status command
    LR1110_GET_VERSION_OC           = 0x0101,  //!< Operation code for get version command
    LR1110_ERASE_FLASH_OC           = 0x8000,  //!< Operation code for flash erase command
    LR1110_ERASE_PAGE_OC            = 0x8001,  //!< Operation code for flash page erase command
    LR1110_WRITE_FLASH_OC           = 0x8002,  //!< Operation code for write flash command
    LR1110_WRITE_FLASH_ENCRYPTED_OC = 0x8003,  //!< Operation code for encrypted flash write command
    LR1110_GET_HASH_OC              = 0x8004,  //!< Operation code for hash getter command
    LR1110_REBOOT_OC                = 0x8005,  //!< Operation code for reboot command
    LR1110_GET_PIN_OC               = 0x800B,  //!< Operation code for PIN read command
    LR1110_GET_CHIP_EUI_OC          = 0x800C,  //!< Operation code for EUI read command
    LR1110_GET_TEMPERATURE          = 0x011A,  //!< Operation code for temperature read command
};

typedef enum
{
    LR1110_WIFI_RESET_CUMUL_TIMING_PHASE_CMD       = 0x07,
    LR1110_WIFI_READ_CUMUL_TIMING_PHASE_CMD        = 0x08,
    LR1110_WIFI_CONFIG_TIMESTAMP_AP_PHONE_CMD      = 0x0B,
    LR1110_WIFI_GET_FIRMWARE_WIFI_VERSION_CMD      = 0x20,
    LR1110_WIFI_PASSIVE_SCAN_CMD                   = 0x30,
    LR1110_WIFI_PASSIVE_SCAN_TIME_LIMIT_CMD        = 0x31,
    LR1110_WIFI_SEARCH_COUNTRY_CODE_CMD            = 0x32,
    LR1110_WIFI_SEARCH_COUNTRY_CODE_TIME_LIMIT_CMD = 0x33,
} lr1110_api_command_wifi_t;

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

/*!
 * @brief TCXO supply voltage values
 */
typedef enum
{
    LR1110_TCXO_CTRL_1_6V = 0x00,  //!< Supply voltage = 1.6v
    LR1110_TCXO_CTRL_1_7V = 0x01,  //!< Supply voltage = 1.7v
    LR1110_TCXO_CTRL_1_8V = 0x02,  //!< Supply voltage = 1.8v
    LR1110_TCXO_CTRL_2_2V = 0x03,  //!< Supply voltage = 2.2v
    LR1110_TCXO_CTRL_2_4V = 0x04,  //!< Supply voltage = 2.4v
    LR1110_TCXO_CTRL_2_7V = 0x05,  //!< Supply voltage = 2.7v
    LR1110_TCXO_CTRL_3_0V = 0x06,  //!< Supply voltage = 3.0v
    LR1110_TCXO_CTRL_3_3V = 0x07,  //!< Supply voltage = 3.3v
} lr1110_tcxo_supply_voltage_t;

typedef struct configuration
{
    GPIO_TypeDef* port;
    uint32_t      pin;
} gpio_t;

typedef struct hal_gpio_irq_s
{
    gpio_t irq1;
    void*                context;
    void ( *callback )( void* context );
} hal_gpio_irq_t;

typedef struct
{
    SPI_TypeDef*    spi;
    gpio_t          nss;
    gpio_t          reset;
    hal_gpio_irq_t  event;
    gpio_t          busy;
} radio_t;