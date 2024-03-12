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

void setLR1110_LoRa_Packet_Type( const void* context, const uint8_t packet_type ) {
    HAL_DBG_TRACE_INFO("Setting LoRa packet type to 0x%0X... ", packet_type);

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

uint8_t getLR1110_LoRa_Packet_Type( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting packet type... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_GET_PACKET_TYPE];
    uint8_t rbuffer[LR1110_LORA_RES_LENGTH_GET_PACKET_TYPE] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_GET_PACKET_TYPE >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_GET_PACKET_TYPE >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_LORA_CMD_LENGTH_GET_PACKET_TYPE, rbuffer, LR1110_LORA_RES_LENGTH_GET_PACKET_TYPE ) == LR1110_SPI_STATUS_OK) {
        switch (rbuffer[1]) {
            case 0:
                HAL_DBG_TRACE_INFO_VALUE("None");
                break;
            case 1:
                HAL_DBG_TRACE_INFO_VALUE("(G)FSK");
                break;
            case 2:
                HAL_DBG_TRACE_INFO_VALUE("LoRa");
                break;
            case 3:
                HAL_DBG_TRACE_INFO_VALUE("Sigfox Uplink");
                break;
            case 4:
                HAL_DBG_TRACE_INFO_VALUE("LR-FHSS");
                break;
            case 5:
                HAL_DBG_TRACE_INFO_VALUE("Ranging");
                break;
            default:
                HAL_DBG_TRACE_ERROR("Unknown packet type");
                break;
        }
        HAL_DBG_TRACE_INFO_VALUE(" (0x%X)\r\n", rbuffer[1]);

        return rbuffer[0];
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get packet type\r\n");
        return -1;
    }
}

void setLR1110_LoRa_Modulation_Params( const void* context, const uint8_t sf, const uint8_t bwl, const uint8_t cr, const uint8_t low_data_rate_ptimize) {
    HAL_DBG_TRACE_INFO("Setting LoRa modulation parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_MODULATION_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_MODULATION_PARAMS >> 0 );
    cbuffer[2] = ( uint8_t ) sf;
    cbuffer[3] = ( uint8_t ) bwl;
    cbuffer[4] = ( uint8_t ) cr;
    cbuffer[5] = ( uint8_t ) low_data_rate_ptimize;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_MODULATION_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa modulation parameters\r\n");
    }
}

void setLR1110_LoRa_Packet_Params( const void* context, const uint8_t pb_lenght_tx1, const uint8_t pb_lenght_tx2, const uint8_t header_type, const uint8_t payload_len, const uint8_t crc, const uint8_t invert_iq) {
    HAL_DBG_TRACE_INFO("Setting LoRa packet parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PACKET_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_PARAMS >> 0 );
    cbuffer[2] = ( uint8_t ) pb_lenght_tx1;
    cbuffer[3] = ( uint8_t ) pb_lenght_tx2;
    cbuffer[4] = ( uint8_t ) header_type;
    cbuffer[5] = ( uint8_t ) crc;           // Cycle Redundancy Check
    cbuffer[6] = ( uint8_t ) invert_iq;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PACKET_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa packet parameters\r\n");
    }
}

// Power Amplifier (PA) configuration
void setLR1110_LoRa_PA_Config( const void* context, const uint8_t pa_sel, const uint8_t reg_pa_supply, const uint8_t pa_duty_cycle, const uint8_t pa_hp_sel) {
    HAL_DBG_TRACE_INFO("Setting LoRa PA configuation... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PA_CONFIG];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PA_CONFIG >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PA_CONFIG >> 0 );
    cbuffer[2] = ( uint8_t ) pa_sel;
    cbuffer[3] = ( uint8_t ) reg_pa_supply;
    cbuffer[4] = ( uint8_t ) pa_duty_cycle;
    cbuffer[5] = ( uint8_t ) pa_hp_sel;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PA_CONFIG ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa PA configuation\r\n");
    }
}

void setLR1110_LoRa_TX_Params( const void* context, const uint8_t tx_power, const uint8_t ramp_time) {
    HAL_DBG_TRACE_INFO("Setting LoRa TX parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_TX_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_TX_PARAMS >> 0 );
    cbuffer[2] = ( uint8_t ) tx_power;
    cbuffer[3] = ( uint8_t ) ramp_time;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa TX parameters\r\n");
    }
}

void setLR1110_LoRa_Public_Network( const void* context, const uint8_t public_network) {
    HAL_DBG_TRACE_INFO("Setting LoRa public network... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PUBLIC_NETWORK];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PUBLIC_NETWORK >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PUBLIC_NETWORK >> 0 );
    cbuffer[2] = ( uint8_t ) public_network;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PUBLIC_NETWORK ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LoRa public network\r\n");
    }
}

void getLR1110_LoRa_Packet_Status( const void* context) {
    HAL_DBG_TRACE_INFO("Getting LoRa packet status... \r\n");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS];
    uint8_t rbuffer[LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_GET_PACKET_STATUS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_GET_PACKET_STATUS >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS, rbuffer, LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_PRINTF("RssiPkt: %d\r\n", (int8_t)rbuffer[1]);
        HAL_DBG_TRACE_PRINTF("SnrPkt: %d\r\n", (int8_t)rbuffer[2]);
        HAL_DBG_TRACE_PRINTF("SignalRssiPkt: %d\r\n", (int8_t)rbuffer[3]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LoRa packet status\r\n");
    }
}

void writeLR1110_Buffer8( const void* context, const uint8_t data) {
    HAL_DBG_TRACE_INFO("Writing 0x%0X to buffer... ", data);

    uint8_t cbuffer[2 + 1];

    cbuffer[0] = ( uint8_t )( 0x0109 >> 8 );
    cbuffer[1] = ( uint8_t )( 0x0109 >> 0 );
    cbuffer[2] = ( uint8_t )( data );

    if ( lr1110_spi_write( context, cbuffer, 2+1 ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to write to buffer");
    }
}

void setLR1110_TX( const void* context, const uint32_t timeout_ms) {
    uint32_t timeout = timeout_ms * 32768;
    HAL_DBG_TRACE_INFO("Setting TX mode with timeout of %d ms... ", timeout_ms);

    uint8_t cbuffer[2+3];

    cbuffer[0] = ( uint8_t )( 0x020A >> 8 );
    cbuffer[1] = ( uint8_t )( 0x020A >> 0 );
    cbuffer[2] = ( uint8_t )( timeout >> 16 );
    cbuffer[3] = ( uint8_t )( timeout >> 8 );
    cbuffer[4] = ( uint8_t )( timeout >> 0 );

    if ( lr1110_spi_write( context, cbuffer, 2+3 ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set TX mode\r\n");
    }
}

void joinAccepts( const void* context, const uint8_t dec_key_id, const uint8_t ver_key_id, const uint8_t lorawan_ver ) {
    HAL_DBG_TRACE_INFO("Joining network... ");

    uint8_t cbuffer[2+3];
    uint8_t rbuffer[1+2] = { 0 };

    cbuffer[0] = ( uint8_t )( 0x0504 >> 8 );
    cbuffer[1] = ( uint8_t )( 0x0504 >> 0 );
    cbuffer[2] = ( uint8_t )( dec_key_id );
    cbuffer[3] = ( uint8_t )( ver_key_id );
    cbuffer[4] = ( uint8_t )( lorawan_ver );

    if (lr1110_spi_read( context, cbuffer, LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS, rbuffer, LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS ) == LR1110_SPI_STATUS_OK) {
        switch (rbuffer[1]) {
        case 0:
            HAL_DBG_TRACE_INFO("0: CRYP_API_SUCCESS. The previous command was successful\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_ERROR("1: CRYP_API_FAIL_CMAC. MIC (first 4 bytes of the CMAC) comparison failed\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_ERROR("3: CRYP_API_INV_KEY_ID. Key/Param Source or Destination ID error\r\n");
            break;
        case 5:
            HAL_DBG_TRACE_ERROR("5: CRYP_API_BUF_SIZE. Data buffer size is invalid. For CryptoAesEncrypt(...) the buffer size must be multiple of 16 bytes\r\n");
            break;
        case 6:
            HAL_DBG_TRACE_ERROR("6: CRYP_API_ERROR. Any other error\r\n");
            break;
        default:
            HAL_DBG_TRACE_ERROR("RFU\r\n")
            break;
        }
        HAL_DBG_TRACE_PRINTF("Data1: %d\r\n", (int8_t)rbuffer[2]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LoRa packet status\r\n");
    }
}
