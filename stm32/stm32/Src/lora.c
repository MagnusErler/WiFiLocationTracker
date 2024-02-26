/**
  ******************************************************************************
  * @file           : lora.c
  * @brief          : Containing all LoRA functions
  ******************************************************************************
  */

#include "lora.h"

#include "spi.h"
#include "main.h"   // for HAL_DBG_TRACE-functions
#include "led.h"

void setLR1110_LoRa_Packet_Type( const void* context ) {
    HAL_DBG_TRACE_INFO("Setting LoRa packet type... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_TYPE >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_TYPE >> 0 );

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa packet type\r\n");
    }
}