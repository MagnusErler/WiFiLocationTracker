#include <stdint.h>

// LENGTHS FOR COMMANDS
#define LR1110_LORA_CMD_LENGTH_NO_PARAM                 2
#define LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE          LR1110_LORA_CMD_LENGTH_NO_PARAM + 1
#define LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS    LR1110_LORA_CMD_LENGTH_NO_PARAM + 4
#define LR1110_LORA_CMD_LENGTH_SET_PACKET_PARAMS        LR1110_LORA_CMD_LENGTH_NO_PARAM + 6
#define LR1110_LORA_CMD_LENGTH_SET_PA_CONFIG            LR1110_LORA_CMD_LENGTH_NO_PARAM + 4
#define LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS            LR1110_LORA_CMD_LENGTH_NO_PARAM + 2
#define LR1110_LORA_CMD_LENGTH_GET_PACKET_TYPE          LR1110_LORA_CMD_LENGTH_NO_PARAM

// LENGTHS FOR RESPONSES
#define LR1110_LORA_LENGTH_NO_PARAM                     1
#define LR1110_LORA_LENGTH_GET_PACKET_TYPE              LR1110_LORA_LENGTH_NO_PARAM + 1

// LR1110 LORA COMMANDS
#define LR1110_LORA_CMD_GET_PACKET_TYPE                 0x0202
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
void setLR1110_LoRa_Packet_Type( const void* context, uint8_t packet_type  );

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
void setLR1110_LoRa_Modulation_Params( const void* context, uint8_t sf, uint8_t bwl, uint8_t cr, uint8_t low_data_rate_ptimize);

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
void setLR1110_LoRa_Packet_Params( const void* context, uint8_t pb_lenght_tx1, uint8_t pb_lenght_tx2, uint8_t header_type, uint8_t payload_len, uint8_t crc, uint8_t invert_iq);

/*!
 * @brief Set the LoRa packet parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] pa_sel PA selection
 * @param [in] reg_pa_supply PA supply
 * @param [in] pa_duty_cycle PA duty cycle
 * @param [in] pa_hp_sel PA HP selection
 */
void setLR1110_LoRa_PA_Config( const void* context, uint8_t pa_sel, uint8_t reg_pa_supply, uint8_t pa_duty_cycle, uint8_t pa_hp_sel);

/*!
 * @brief Set the LoRa packet parameters
 *
 * @param [in] context Radio abstraction
 * @param [in] tx_power TX power
 * @param [in] ramp_time Ramp time
 */
void setLR1110_LoRa_TX_Params( const void* context, uint8_t tx_power, uint8_t ramp_time);