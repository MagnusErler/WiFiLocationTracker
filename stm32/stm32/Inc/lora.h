#include <stdint.h>

// LENGTHS FOR COMMANDS
#define LR1110_LORA_CMD_LENGTH_NO_PARAM                 2
#define LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE          LR1110_LORA_CMD_LENGTH_NO_PARAM + 1
#define LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS    LR1110_LORA_CMD_LENGTH_NO_PARAM + 4
#define LR1110_LORA_CMD_LENGTH_SET_PACKET_PARAMS        LR1110_LORA_CMD_LENGTH_NO_PARAM + 6
#define LR1110_LORA_CMD_LENGTH_SET_PA_CONFIG            LR1110_LORA_CMD_LENGTH_NO_PARAM + 4
#define LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS            LR1110_LORA_CMD_LENGTH_NO_PARAM + 2
#define LR1110_LORA_CMD_LENGTH_GET_PACKET_TYPE          LR1110_LORA_CMD_LENGTH_NO_PARAM
#define LR1110_LORA_CMD_LENGTH_SET_PUBLIC_NETWORK       LR1110_LORA_CMD_LENGTH_NO_PARAM + 1
#define LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS        LR1110_LORA_CMD_LENGTH_NO_PARAM
#define LR1110_LORA_CMD_LENGTH_SET_RF_FREQUENCY         LR1110_LORA_CMD_LENGTH_NO_PARAM + 4
#define LR1110_LORA_CMD_LENGTH_GET_STATUS               LR1110_LORA_CMD_LENGTH_NO_PARAM

// LENGTHS FOR RESPONSES
#define LR1110_LORA_RES_LENGTH_NO_PARAM                     1
#define LR1110_LORA_RES_LENGTH_GET_PACKET_TYPE              LR1110_LORA_RES_LENGTH_NO_PARAM + 1
#define LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS            LR1110_LORA_RES_LENGTH_NO_PARAM + 3
#define LR1110_LORA_RES_LENGTH_GET_STATUS                   LR1110_LORA_RES_LENGTH_NO_PARAM + 8

// LR1110 LORA COMMANDS
#define LR1110_LORA_CMD_GET_STATUS                      0x0201
#define LR1110_LORA_CMD_GET_PACKET_TYPE                 0x0202
#define LR1110_LORA_CMD_GET_PACKET_STATUS               0x0204
#define LR1110_LORA_CMD_SET_PUBLIC_NETWORK              0x0208
#define LR1110_LORA_CMD_SET_RF_FREQUENCY                0x020B
#define LR1110_LORA_CMD_SET_PACKET_TYPE                 0x020E
#define LR1110_LORA_CMD_SET_MODULATION_PARAMS           0x020F
#define LR1110_LORA_CMD_SET_PACKET_PARAMS               0x0210
#define LR1110_LORA_CMD_SET_PA_CONFIG                   0x0215
#define LR1110_LORA_CMD_SET_TX_PARAMS                   0x0211

/*!
 * @brief Set the LoRa packet type
 *
 * @param [in] context Radio abstraction
 * @param [in] packet_type LoRa packet type
 */
void setLR1110_LoRa_Packet_Type( const void* context, const uint8_t packet_type  );

/*!
 * @brief Get the LoRa packet type
 *
 * @param [in] context Radio abstraction
 * 
 * @return LoRa packet type
 */
uint8_t getLR1110_LoRa_Packet_Type( const void* context );

/*!
 * @brief Set the LoRa modulation parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] sf Spreading factor
 * @param [in] bwl Bandwidth
 * @param [in] cr Coding rate
 * @param [in] low_data_rate_ptimize Low data rate optimize
 */
void setLR1110_LoRa_Modulation_Params( const void* context, const uint8_t sf, const uint8_t bwl, const uint8_t cr, const uint8_t low_data_rate_ptimize);

/*!
 * @brief Set the LoRa packet parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] pb_lenght_tx1 Payload length for TX
 * @param [in] pb_lenght_rx2 Payload length for RX
 * @param [in] header_type Header type
 * @param [in] crc CRC
 * @param [in] invert_iq Invert IQ
 */
void setLR1110_LoRa_Packet_Params( const void* context, const uint8_t pb_lenght_tx1, const uint8_t pb_lenght_tx2, const uint8_t header_type, const uint8_t payload_len, const uint8_t crc, const uint8_t invert_iq);

/*!
 * @brief Set the LoRa PA configuration
 *
 * @param [in] context Radio abstraction
 * @param [in] pa_sel PA selection
 * @param [in] reg_pa_supply PA supply
 * @param [in] pa_duty_cycle PA duty cycle
 * @param [in] pa_hp_sel PA HP selection
 */
void setLR1110_LoRa_PA_Config( const void* context, const uint8_t pa_sel, const uint8_t reg_pa_supply, const uint8_t pa_duty_cycle, const uint8_t pa_hp_sel);

/*!
 * @brief Set the LoRa TX parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] tx_power TX power
 * @param [in] ramp_time Ramp time
 */
void setLR1110_LoRa_TX_Params( const void* context, const uint8_t tx_power, const uint8_t ramp_time);

/*!
 * @brief Set the LoRa public network
 *
 * @param [in] context Radio abstraction
 * @param [in] public_network Public network
 */
void setLR1110_LoRa_Public_Network( const void* context, const uint8_t public_network);

/*!
 * @brief Get the LoRa packet status
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_LoRa_Packet_Status( const void* context);


/*!
 * @brief Write to the LR1110 buffer
 *
 * @param [in] context Radio implementation parameters
 * @param [in] data    Data to be written
 */
void writeLR1110_Buffer8( const void* context, const uint8_t data);

/*!
 * @brief Set the LoRa TX
 *
 * @param [in] context Radio abstraction
 * @param [in] timeout Timeout
 */
void setLR1110_TX( const void* context, const uint32_t timeout);

/*!
 * @brief Set the LoRa TX
 *
 * @param [in] context Radio abstraction
 * @param [in] rf_frequency RF frequency
 */
void setLR1110_RF_Frequency( const void* context, const uint32_t rf_frequency);

/*!
 * @brief Get the RF status
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_RF_Status( const void* context);

void joinAccepts( const void* context, const uint8_t dec_key_id, const uint8_t ver_key_id, const uint8_t lorawan_ver );

void setLR1110_Crypto_Key( const void* context );

void getLR1110_MIC( const void* context );