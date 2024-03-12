#include <stdint.h>

// LENGTHS FOR COMMANDS
#define LR1110_CMD_LENGTH_NO_PARAM   2
#define LR1110_CMD_LENGTH_GET_VERSION           LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_GET_TEMPERATURE       LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_GET_CHIPEUI           LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_GET_SEMTECH_JOINEUI   LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_GET_ROOT_KEY_AND_PIN  LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_GET_BATTERY_VOLTAGE   LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_GET_STATUS            LR1110_CMD_LENGTH_NO_PARAM + 4
#define LR1110_CMD_LENGTH_GET_ERRORS            LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_SET_DIO_IRQ_PARAMS    LR1110_CMD_LENGTH_NO_PARAM + 8
#define LR1110_CMD_LENGTH_SET_STANDBY_MODE      LR1110_CMD_LENGTH_NO_PARAM + 1
#define LR1110_CMD_LENGTH_SET_TCXO_MODE         LR1110_CMD_LENGTH_NO_PARAM + 4
#define LR1110_CMD_LENGTH_CLEAR_RX_BUFFER       LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_CALIBRATE_IMAGE       LR1110_CMD_LENGTH_NO_PARAM + 2
#define LR1110_CMD_LENGTH_CALIBRATE             LR1110_CMD_LENGTH_NO_PARAM + 1
#define LR1110_CMD_LENGTH_CLEAR_ERRORS          LR1110_CMD_LENGTH_NO_PARAM
#define LR1110_CMD_LENGTH_REBOOT                LR1110_CMD_LENGTH_NO_PARAM + 1
#define LR1110_CMD_LENGTH_CLEAR_INTERRUPTS      LR1110_CMD_LENGTH_NO_PARAM + 4

// LENGTHS FOR RESPONSES
#define LR1110_RES_LENGTH_NO_PARAM              1
#define LR1110_RES_LENGTH_GET_VERSION           LR1110_RES_LENGTH_NO_PARAM + 4
#define LR1110_RES_LENGTH_GET_TEMPERATURE       LR1110_RES_LENGTH_NO_PARAM + 2
#define LR1110_RES_LENGTH_GET_CHIPEUI           LR1110_RES_LENGTH_NO_PARAM + 8
#define LR1110_RES_LENGTH_GET_SEMTECH_JOINEUI   LR1110_RES_LENGTH_NO_PARAM + 8
#define LR1110_RES_LENGTH_GET_ROOT_KEY_AND_PIN  LR1110_CMD_LENGTH_NO_PARAM + 4
#define LR1110_RES_LENGTH_GET_ERRORS            LR1110_RES_LENGTH_NO_PARAM + 2
#define LR1110_RES_LENGTH_GET_BATTERY_VOLTAGE   LR1110_RES_LENGTH_NO_PARAM + 1

// LR1110 CHIP COMMANDS
#define LR1110_CMD_GET_STATUS                   0x0100
#define LR1110_CMD_GET_VERSION                  0x0101
#define LR1110_CMD_CLEAR_RX_BUFFER              0x010B
#define LR1110_CMD_GET_ERRORS                   0x010D
#define LR1110_CMD_CLEAR_ERRORS                 0x010E
#define LR1110_CMD_CALIBRATE                    0x010F
#define LR1110_CMD_CALIBRATE_IMAGE              0x0111
#define LR1110_CMD_SET_DIO_IRQ_PARAMS           0x0113
#define LR1110_CMD_CLEAR_INTERRUPTS             0x0114
#define LR1110_CMD_SET_TCXO_MODE                0x0117
#define LR1110_CMD_REBOOT                       0x0118
#define LR1110_CMD_GET_BATTERY_VOLTAGE          0x0119
#define LR1110_CMD_GET_TEMPERATURE              0x011A
#define LR1110_CMD_SET_STANDBY_MODE             0x011C
#define LR1110_CMD_GET_CHIPEUI                  0x0125
#define LR1110_CMD_GET_SEMTECH_JOINEUI          0x0126
#define LR1110_CMD_GET_ROOT_KEY_AND_PIN         0x0127

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
 * @param [out] chip_eui ChipEUI
 */
void getLR1110_ChipEUI( const void* context, uint8_t* chip_eui );

/*!
 * @brief Get LR1110 Semtech JoinEui
 *
 * @param [in] context Radio abstraction
 * @param [out] join_eui JoinEUI
 */
void getLR1110_Semtech_JoinEui( const void* context, uint8_t* join_eui  );

/*!
 * @brief Get LR1110 Root Keys and Pin
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_Root_Keys_And_Pin( const void* context );

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
void setLR1110_TCXO_Mode( const void* context );

/*!
 * @brief Reset LR1110
 *
 * @param [in] context Radio abstraction
 * @param [in] reset_type Type of reset
 */
void resetLR1110( const void* context, const uint8_t reset_type);

/*!
 * @brief Get LR1110 status
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_Status( const void* context );

/*!
 * @brief Get LR1110 errors (non command errors)
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_Errors( const void* context );

/*!
 * @brief Clear LR1110 errors
 *
 * @param [in] context Radio abstraction
 */
void clearLR1110_Errors( const void* context);

/*!
 * @brief Clear LR1110 IRQ
 *
 * @param [in] context Radio abstraction
 */
void clearLR1110_IRQ( const void* context);

/*!
 * @brief Set LR1110 DIO IRQ parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] irq1_to_enable DIO1 IRQ to enable
 * @param [in] irq2_to_enable DIO2 IRQ to enable
 */
void setLR1110_Dio_Irq_Params( const void* context, const uint64_t irq1_to_enable, const uint64_t irq2_to_enable);

/*!
 * @brief Calibrate LR1110 image
 *
 * @param [in] context Radio abstraction
 * @param [in] freq1 Frequency 1
 * @param [in] freq2 Frequency 2
 */
void calibrateLR1110_Image( const void* context, const uint8_t freq1, const uint8_t freq2);

/*!
 * @brief Calibrate LR1110 image
 *
 * @param [in] context Radio abstraction
 * @param [in] calib_params Calibration parameters
 */
void calibrateLR1110( const void* context, const uint8_t calib_params);

/*!
 * @brief Set LR1110 Standby Mode
 *
 * @param [in] context Radio abstraction
 * @param [in] stdby_config Standby configuration
 */
void setLR1110_Standby_Mode( const void* context, const uint8_t stdby_config);

/*!
 * @brief Clear LR1110 RX Buffer
 *
 * @param [in] context Radio abstraction
 */
void clearLR1110_RX_Buffer( const void* context);

