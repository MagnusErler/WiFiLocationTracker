#include <stdint.h>

// LENGTHS FOR COMMANDS
#define LR1110_CMD_NO_PARAM_LENGTH              2
#define LR1110_GET_VERSION_CMD_LENGTH           LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_TEMPERATURE_CMD_LENGTH       LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_REBOOT_CMD_LENGTH                LR1110_CMD_NO_PARAM_LENGTH + 1
#define LR1110_GET_CHIPEUI_CMD_LENGTH           LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_CALIBRATE_CMD_LENGTH             LR1110_CMD_NO_PARAM_LENGTH + 2
#define LR1110_GET_SEMTECH_JOINEUI_CMD_LENGTH   LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_BATTERY_VOLTAGE_CMD_LENGTH   LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_STATUS_CMD_LENGTH            LR1110_CMD_NO_PARAM_LENGTH + 4
#define LR1110_GET_ERRORS_CMD_LENGTH            LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_SET_DIO_IRQ_PARAMS_CMD_LENGTH    LR1110_CMD_NO_PARAM_LENGTH + 8
#define LR1110_SET_TCXO_MODE_CMD_LENGTH         LR1110_CMD_NO_PARAM_LENGTH + 4

// LENGTHS FOR RESPONSES
#define LR1110_NO_PARAM_LENGTH                  1
#define LR1110_GET_VERSION_LENGTH               LR1110_NO_PARAM_LENGTH + 3
#define LR1110_GET_TEMPERATURE_LENGTH           LR1110_NO_PARAM_LENGTH + 1
#define LR1110_GET_CHIPEUI_LENGTH               LR1110_NO_PARAM_LENGTH + 7
#define LR1110_GET_SEMTECH_JOINEUI_LENGTH       LR1110_NO_PARAM_LENGTH + 7
#define LR1110_GET_ERRORS_LENGTH                LR1110_NO_PARAM_LENGTH + 2
#define LR1110_GET_BATTERY_VOLTAGE_LENGTH       LR1110_NO_PARAM_LENGTH

// LR1110 CHIP COMMANDS
#define LR1110_GET_STATUS_CMD                   0x0100
#define LR1110_GET_VERSION_CMD                  0x0101
#define LR1110_GET_ERRORS_CMD                   0x010D
#define LR1110_CALIBRATE_CMD                    0x0111
#define LR1110_SET_DIO_IRQ_PARAMS_CMD           0x0113
#define LR1110_SET_TCXO_MODE_CMD                0x0117
#define LR1110_REBOOT_CMD                       0x0118
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
 * @param [in] reset_type Type of reset
 */
void resetLR1110( const void* context, uint8_t reset_type);

/*!
 * @brief Get LR1110 status
 *
 * @param [in] context Radio abstraction
 */
void getStatus( const void* context );

/*!
 * @brief Get LR1110 errors
 *
 * @param [in] context Radio abstraction
 */
void getErrors( const void* context );

/*!
 * @brief Set LR1110 DIO IRQ parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] irq1_to_enable DIO1 IRQ to enable
 * @param [in] irq2_to_enable DIO2 IRQ to enable
 */
void setLR1110_Dio_Irq_Params( const void* context, uint64_t irq1_to_enable, uint64_t irq2_to_enable);

/*!
 * @brief Calibrate LR1110 image
 *
 * @param [in] context Radio abstraction
 * @param [in] freq1 Frequency 1
 * @param [in] freq2 Frequency 2
 */
void calibLR1110_Image( const void* context, uint8_t freq1, uint8_t freq2)