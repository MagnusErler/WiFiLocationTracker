

// LENGTHS FOR COMMANDS
#define LR1110_LORA_CMD_LENGTH_NO_PARAM                 2
#define LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE          LR1110_LORA_CMD_LENGTH_NO_PARAM + 1
#define LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS    LR1110_LORA_CMD_LENGTH_NO_PARAM + 4

// LENGTHS FOR RESPONSES
#define LR1110_LORA_LENGTH_NO_PARAM                     1

// LR1110 WIFI COMMANDS
#define LR1110_LORA_CMD_SET_PACKET_TYPE                 0x020E
#define LR1110_LORA_CMD_SET_MODULATION_PARAMS           0x020F

/*!
 * @brief Set the LoRa packet type
 *
 * @param [in] context Radio abstraction
 * @param [in] packet_type LoRa packet type
 */
void setLR1110_LoRa_Packet_Type( const void* context, uint8_t packet_type  );

/*!
 * @brief Set the LoRa modulation parameters
 *
 * @param [in] context Radio abstraction
 */
void setLR1110_LoRa_Modulation_Params( const void* context, uint8_t sf, uint8_t bwl, uint8_t cr, uint8_t low_data_rate_ptimize);