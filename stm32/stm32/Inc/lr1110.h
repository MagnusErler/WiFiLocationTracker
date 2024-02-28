
// LENGTHS FOR COMMANDS
#define LR1110_CMD_NO_PARAM_LENGTH              2
#define LR1110_GET_VERSION_CMD_LENGTH           LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_TEMPERATURE_CMD_LENGTH       LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_CHIPEUI_CMD_LENGTH            LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_SEMTECH_JOINEUI_CMD_LENGTH   LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_BATTERY_VOLTAGE_CMD_LENGTH   LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_SET_TCXO_MODE_CMD_LENGTH         LR1110_CMD_NO_PARAM_LENGTH + 4

// LENGTHS FOR RESPONSES
#define LR1110_NO_PARAM_LENGTH                  1
#define LR1110_GET_VERSION_LENGTH               LR1110_NO_PARAM_LENGTH + 3
#define LR1110_GET_TEMPERATURE_LENGTH           LR1110_NO_PARAM_LENGTH + 1
#define LR1110_GET_CHIPEUI_LENGTH                LR1110_NO_PARAM_LENGTH + 7
#define LR1110_GET_SEMTECH_JOINEUI_LENGTH       LR1110_NO_PARAM_LENGTH + 7
#define LR1110_GET_BATTERY_VOLTAGE_LENGTH       LR1110_NO_PARAM_LENGTH

// LR1110 CHIP COMMANDS
#define LR1110_GET_VERSION_CMD                  0x0101
#define LR1110_SET_TCXO_MODE_CMD                0x0117
#define LR1110_GET_BATTERY_VOLTAGE_CMD          0x0119
#define LR1110_GET_TEMPERATURE_CMD              0x011A
#define LR1110_GET_CHIPEUI_CMD                  0x0125
#define LR1110_GET_SEMTECH_JOINEUI_CMD          0x0126

/*!
 * @brief Get LR1110 version
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_Bootloader_Version( const void* context );

/*!
 * @brief Get LR1110 temperature
 *
 * @param [in] context Radio abstraction
 * 
 * @return temperature in Celsius
 */
float getLR1110_Temperature( const void* context );

/*!
 * @brief Get LR1110 ChipEUI
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_ChipEUI( const void* context );

/*!
 * @brief Get LR1110 Semtech JoinEui
 *
 * @param [in] context Radio abstraction
 * 
 */
void getLR1110_Semtech_JoinEui( const void* context );

/*!
 * @brief Get LR1110 Battery Voltage
 *
 * @param [in] context Radio abstraction
 * 
 * @return battery voltage in Volts
 */
float getLR1110_Battery_Voltage( const void* context );

/*!
 * @brief Setup LR1110 TCXO
 *
 * @param [in] context Radio abstraction
 */
void setupLR1110_TCXO( const void* context );

/*!
 * @brief Reset LR1110
 *
 * @param [in] context Radio abstraction
 */
void resetLR1110( const void* context );