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

void setLR1110_LoRa_Packet_Type( const void* context, uint8_t packet_type ) {
    HAL_DBG_TRACE_INFO("Setting LoRa packet type... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_TYPE >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_TYPE >> 0 );
    cbuffer[2] = packet_type;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa packet type\r\n");
    }
}

void setLR1110_LoRa_Modulation_Params( const void* context, uint8_t sf, uint8_t bwl, uint8_t cr, uint8_t low_data_rate_ptimize) {
    HAL_DBG_TRACE_INFO("Setting LoRa modulation parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_MODULATION_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_MODULATION_PARAMS >> 0 );
    cbuffer[2] = sf;
    cbuffer[3] = bwl;
    cbuffer[4] = cr;
    cbuffer[5] = low_data_rate_ptimize;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa modulation parameters\r\n");
    }
}

void setLR1110_LoRa_Packet_Params( const void* context, uint8_t pb_lenght_tx1, uint8_t pb_lenght_tx2, uint8_t header_type, uint8_t crc, uint8_t invert_iq) {
    HAL_DBG_TRACE_INFO("Setting LoRa packet parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PACKET_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_PARAMS >> 0 );
    cbuffer[2] = pb_lenght_tx1;
    cbuffer[3] = pb_lenght_tx2;
    cbuffer[4] = header_type;
    cbuffer[5] = crc;
    cbuffer[6] = invert_iq;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PACKET_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa packet parameters\r\n");
    }
}

void setLR1110_LoRa_PA_Config( const void* context, uint8_t pa_sel, uint8_t reg_pa_supply, uint8_t pa_duty_cycle, uint8_t pa_hp_sel) {
    HAL_DBG_TRACE_INFO("Setting LoRa PA configuation... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PA_CONFIG];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PA_CONFIG >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PA_CONFIG >> 0 );
    cbuffer[2] = pa_sel;
    cbuffer[3] = reg_pa_supply;
    cbuffer[4] = pa_duty_cycle;
    cbuffer[5] = pa_hp_sel;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PA_CONFIG ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa PA configuation\r\n");
    }
}

void setLR1110_LoRa_TX_Params( const void* context, uint8_t tx_power, uint8_t ramp_time) {
    HAL_DBG_TRACE_INFO("Setting LoRa TX parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_TX_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_TX_PARAMS >> 0 );
    cbuffer[2] = tx_power;
    cbuffer[3] = ramp_time;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa TX parameters\r\n");
    }
}



//gfsk
//Bitrate (bps): 150000
// Freq dev (hz): 50000
//


