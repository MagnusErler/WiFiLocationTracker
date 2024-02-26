

// LENGTHS FOR COMMANDS
#define LR1110_LORA_CMD_LENGTH_NO_PARAM             2
#define LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE      LR1110_LORA_CMD_LENGTH_NO_PARAM + 1

// LENGTHS FOR RESPONSES
#define LR1110_LORA_LENGTH_NO_PARAM                 1

// LR1110 WIFI COMMANDS
#define LR1110_LORA_CMD_SET_PACKET_TYPE             0x020E

/*!
 * @brief Set the LoRa packet type
 *
 * @param [in] context Radio abstraction
 */
void setLR1110_LoRa_Packet_Type( const void* context );