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
    HAL_DBG_TRACE_INFO("Setting LR1110 LoRa packet type to 0x%0X... ", packet_type);

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_TYPE >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_PACKET_TYPE >> 0 );
    cbuffer[2] = packet_type;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_PACKET_TYPE ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 LoRa packet type\r\n");
    }
}

uint8_t getLR1110_LoRa_Packet_Type( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting LR1110 packet type... ");

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

        return rbuffer[1];
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LR1110 packet type\r\n");
        return -1;
    }
}

void setLR1110_LoRa_Modulation_Params( const void* context, const uint8_t sf, const uint8_t bwl, const uint8_t cr, const uint8_t low_data_rate_ptimize) {
    HAL_DBG_TRACE_INFO("Setting LR1110 LoRa modulation parameters... ");

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
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 LoRa modulation parameters\r\n");
    }
}

void setLR1110_LoRa_Packet_Params( const void* context, const uint8_t pb_lenght_tx1, const uint8_t pb_lenght_tx2, const uint8_t header_type, const uint8_t payload_len, const uint8_t crc, const uint8_t invert_iq) {
    HAL_DBG_TRACE_INFO("Setting LR1110 LoRa packet parameters... ");

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
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 LoRa packet parameters\r\n");
    }
}

// Power Amplifier (PA) configuration
void setLR1110_LoRa_PA_Config( const void* context, const uint8_t pa_sel, const uint8_t reg_pa_supply, const uint8_t pa_duty_cycle, const uint8_t pa_hp_sel) {
    HAL_DBG_TRACE_INFO("Setting LR1110 LoRa PA configuation... ");

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
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 LoRa PA configuation\r\n");
    }
}

void setLR1110_LoRa_TX_Params( const void* context, const uint8_t tx_power, const uint8_t ramp_time) {
    HAL_DBG_TRACE_INFO("Setting LR1110 LoRa TX parameters... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_TX_PARAMS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_TX_PARAMS >> 0 );
    cbuffer[2] = ( uint8_t ) tx_power;
    cbuffer[3] = ( uint8_t ) ramp_time;

    if ( lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_TX_PARAMS ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 LoRa TX parameters\r\n");
    }
}

void setLR1110_LoRa_Public_Network( const void* context, const uint8_t public_network) {
    HAL_DBG_TRACE_INFO("Setting LR1110 LoRa public network... ");

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
    HAL_DBG_TRACE_INFO("Getting LR1110 LoRa packet status... \r\n");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS];
    uint8_t rbuffer[LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_GET_PACKET_STATUS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_GET_PACKET_STATUS >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS, rbuffer, LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_PRINTF("RssiPkt: %d\r\n", (int8_t)rbuffer[1]);
        HAL_DBG_TRACE_PRINTF("SnrPkt: %d\r\n", (int8_t)rbuffer[2]);
        HAL_DBG_TRACE_PRINTF("SignalRssiPkt: %d\r\n", (int8_t)rbuffer[3]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LR1110 LoRa packet status\r\n");
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
    HAL_DBG_TRACE_INFO("Setting LR1110 TX mode with timeout of %d ms... ", timeout_ms);

    uint8_t cbuffer[2+3];

    cbuffer[0] = ( uint8_t )( 0x020A >> 8 );
    cbuffer[1] = ( uint8_t )( 0x020A >> 0 );
    cbuffer[2] = ( uint8_t )( timeout >> 16 );
    cbuffer[3] = ( uint8_t )( timeout >> 8 );
    cbuffer[4] = ( uint8_t )( timeout >> 0 );

    if ( lr1110_spi_write( context, cbuffer, 2+3 ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 TX mode\r\n");
    }
}

void getLR1110_RF_Status( const void* context) {
    HAL_DBG_TRACE_INFO("Getting LR1110 RF status... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_GET_STATUS];
    uint8_t rbuffer[LR1110_LORA_RES_LENGTH_GET_STATUS] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_GET_STATUS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_GET_STATUS >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_LORA_CMD_LENGTH_GET_PACKET_STATUS, rbuffer, LR1110_LORA_RES_LENGTH_GET_PACKET_STATUS ) == LR1110_SPI_STATUS_OK) {
        // combine rbuffer[1] and rbuffer[2] to get received packet
        HAL_DBG_TRACE_PRINTF("\r\nReceived packet: %d\r\n", (int16_t)(rbuffer[1] << 8 | rbuffer[2]));
        HAL_DBG_TRACE_PRINTF("Received packet with a CRC error: %d\r\n", (int16_t)(rbuffer[3] << 8 | rbuffer[4]));
        HAL_DBG_TRACE_PRINTF("Number of packets with a Header error: %d\r\n", (int16_t)(rbuffer[5] << 8 | rbuffer[6]));
        HAL_DBG_TRACE_PRINTF("Number of false synchronizations.: %d\r\n", (int16_t)(rbuffer[7] << 8 | rbuffer[8]));
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LR1110 LoRa packet status\r\n");
    }
}

void joinAccepts( const void* context, const uint8_t dec_key_id, const uint8_t ver_key_id, const uint8_t lorawan_ver ) {
    HAL_DBG_TRACE_INFO("Joining network... ");

    uint8_t cbuffer[2+3+1+16];
    uint8_t rbuffer[1+2] = { 0 };

    cbuffer[0] = ( uint8_t )( 0x0504 >> 8 );
    cbuffer[1] = ( uint8_t )( 0x0504 >> 0 );

    cbuffer[2] = ( uint8_t ) dec_key_id;
    cbuffer[3] = ( uint8_t ) ver_key_id;
    cbuffer[4] = ( uint8_t ) lorawan_ver;

    cbuffer[5] = ( uint8_t ) 16 ;

    // cbuffer[6] = ( uint8_t ) ( 0x07 >> 0 );
    // cbuffer[7] = ( uint8_t ) ( 0x0A >> 0 );
    // cbuffer[8] = ( uint8_t ) ( 0x16 >> 0 );
    // cbuffer[9] = ( uint8_t ) ( 0xB4 >> 0 );
    // cbuffer[10] = ( uint8_t ) ( 0x2E >> 0 );
    // cbuffer[11] = ( uint8_t ) ( 0x40 >> 0 );
    // cbuffer[12] = ( uint8_t ) ( 0x9F >> 0 );
    // cbuffer[13] = ( uint8_t ) ( 0x96 >> 0 );
    // cbuffer[14] = ( uint8_t ) ( 0xE9 >> 0 );
    // cbuffer[15] = ( uint8_t ) ( 0x3D >> 0 );
    // cbuffer[16] = ( uint8_t ) ( 0x7E >> 0 );
    // cbuffer[17] = ( uint8_t ) ( 0x11 >> 0 );
    // cbuffer[18] = ( uint8_t ) ( 0x73 >> 0 );
    // cbuffer[19] = ( uint8_t ) ( 0x93 >> 0 );
    // cbuffer[20] = ( uint8_t ) ( 0x17 >> 0 );
    // cbuffer[21] = ( uint8_t ) ( 0x2A >> 0 );
    cbuffer[6] = ( uint8_t ) ( 0x07 >> 0 );
    cbuffer[7] = ( uint8_t ) ( 0x0A >> 0 );
    cbuffer[8] = ( uint8_t ) ( 0x16 >> 0 );
    cbuffer[9] = ( uint8_t ) ( 0xB4 >> 0 );
    cbuffer[10] = ( uint8_t ) ( 0x07 >> 0 );
    cbuffer[11] = ( uint8_t ) ( 0x0A >> 0 );
    cbuffer[12] = ( uint8_t ) ( 0x16 >> 0 );
    cbuffer[13] = ( uint8_t ) ( 0xB4 >> 0 );
    cbuffer[14] = ( uint8_t ) ( 0x07 >> 0 );
    cbuffer[15] = ( uint8_t ) ( 0x0A >> 0 );
    cbuffer[16] = ( uint8_t ) ( 0x16 >> 0 );
    cbuffer[17] = ( uint8_t ) ( 0xB4 >> 0 );
    cbuffer[18] = ( uint8_t ) ( 0x07 >> 0 );
    cbuffer[19] = ( uint8_t ) ( 0x0A >> 0 );
    cbuffer[20] = ( uint8_t ) ( 0x16 >> 0 );
    cbuffer[21] = ( uint8_t ) ( 0xB4 >> 0 );

    if (lr1110_spi_read( context, cbuffer, 2+3+1+16, rbuffer, 1+2 ) == LR1110_SPI_STATUS_OK) {
        switch (rbuffer[1]) {
        case 0:
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            //HAL_DBG_TRACE_INFO_VALUE("0: CRYP_API_SUCCESS. The previous command was successful\r\n");
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
        HAL_DBG_TRACE_ERROR("Failed to join network\r\n");
    }
}

void setLR1110_Crypto_Key( const void* context ) {
    HAL_DBG_TRACE_INFO("Setting LR1110 crypto key... ");

    uint8_t cbuffer[2+1+16];
    uint8_t rbuffer[1+1] = { 0 };

    cbuffer[0] = ( uint8_t )( 0x0502 >> 8 );
    cbuffer[1] = ( uint8_t )( 0x0502 >> 0 );

    cbuffer[2] = ( uint8_t ) 5;

    cbuffer[3] = ( uint8_t ) ( 0x2b >> 0 );
    cbuffer[4] = ( uint8_t ) ( 0x7e >> 0 );
    cbuffer[5] = ( uint8_t ) ( 0x15 >> 0 );
    cbuffer[6] = ( uint8_t ) ( 0x16 >> 0 );
    cbuffer[7] = ( uint8_t ) ( 0x28 >> 0 );
    cbuffer[8] = ( uint8_t ) ( 0xae >> 0 );
    cbuffer[9] = ( uint8_t ) ( 0xd2 >> 0 );
    cbuffer[10] = ( uint8_t ) ( 0xa6 >> 0 );
    cbuffer[11] = ( uint8_t ) ( 0xab >> 0 );
    cbuffer[12] = ( uint8_t ) ( 0xf7 >> 0 );
    cbuffer[13] = ( uint8_t ) ( 0x15 >> 0 );
    cbuffer[14] = ( uint8_t ) ( 0x88 >> 0 );
    cbuffer[15] = ( uint8_t ) ( 0x09 >> 0 );
    cbuffer[16] = ( uint8_t ) ( 0xcf >> 0 );
    cbuffer[17] = ( uint8_t ) ( 0x4f >> 0 );
    cbuffer[18] = ( uint8_t ) ( 0x3c >> 0 );

    if (lr1110_spi_read( context, cbuffer, 2+1+16, rbuffer, 1+1 ) == LR1110_SPI_STATUS_OK) {
        switch (rbuffer[1]) {
        case 0:
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            //HAL_DBG_TRACE_INFO_VALUE("0: CRYP_API_SUCCESS. The previous command was successful\r\n");
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
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 crypto key\r\n");
    }
}

void getLR1110_MIC( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting LR1110 MIC... ");

    uint8_t cbuffer[2+1+16];
    uint8_t rbuffer[1+5] = { 0 };

    cbuffer[0] = ( uint8_t )( 0x0505 >> 8 );
    cbuffer[1] = ( uint8_t )( 0x0505 >> 0 );

    cbuffer[2] = ( uint8_t ) 5;

    // 6BC1BEE2 2E409F96 E93D7E11 7393172A
    // 2b7e1516 28aed2a6abf71588 09cf4f3c
    cbuffer[3] = ( uint8_t ) ( 0x6B >> 0 );
    cbuffer[4] = ( uint8_t ) ( 0xC1 >> 0 );
    cbuffer[5] = ( uint8_t ) ( 0xBE >> 0 );
    cbuffer[6] = ( uint8_t ) ( 0xE2 >> 0 );
    cbuffer[7] = ( uint8_t ) ( 0x2E >> 0 );
    cbuffer[8] = ( uint8_t ) ( 0x40 >> 0 );
    cbuffer[9] = ( uint8_t ) ( 0x9F >> 0 );
    cbuffer[10] = ( uint8_t ) ( 0x96 >> 0 );
    cbuffer[11] = ( uint8_t ) ( 0xE9 >> 0 );
    cbuffer[12] = ( uint8_t ) ( 0x3D >> 0 );
    cbuffer[13] = ( uint8_t ) ( 0x7E >> 0 );
    cbuffer[14] = ( uint8_t ) ( 0x11 >> 0 );
    cbuffer[15] = ( uint8_t ) ( 0x73 >> 0 );
    cbuffer[16] = ( uint8_t ) ( 0x93 >> 0 );
    cbuffer[17] = ( uint8_t ) ( 0x17 >> 0 );
    cbuffer[18] = ( uint8_t ) ( 0x2A >> 0 );

    if (lr1110_spi_read( context, cbuffer, 2+1+16, rbuffer, 1+5 ) == LR1110_SPI_STATUS_OK) {
        switch (rbuffer[1]) {
        case 0:
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            //HAL_DBG_TRACE_INFO_VALUE("0: CRYP_API_SUCCESS. The previous command was successful\r\n");
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
        HAL_DBG_TRACE_INFO_VALUE("MIC1: 0x%X\r\n", rbuffer[2]);
        HAL_DBG_TRACE_INFO_VALUE("MIC2: 0x%X\r\n", rbuffer[3]);
        HAL_DBG_TRACE_INFO_VALUE("MIC3: 0x%X\r\n", rbuffer[4]);
        HAL_DBG_TRACE_INFO_VALUE("MIC4: 0x%X\r\n", rbuffer[5]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LR1110 MIC\r\n");
    }
}

void getLR1110_Encrypted_Data( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting LR1110 encrypted data... ");

    uint8_t cbuffer[2+1+16];
    uint8_t rbuffer[1+1+16] = { 0 };

    cbuffer[0] = ( uint8_t )( 0x0507 >> 8 );
    cbuffer[1] = ( uint8_t )( 0x0507 >> 0 );

    cbuffer[2] = ( uint8_t ) 5;

    // 6BC1BEE2 2E409F96 E93D7E11 7393172A
    cbuffer[3] = ( uint8_t ) ( 0x6B >> 0 );
    cbuffer[4] = ( uint8_t ) ( 0xC1 >> 0 );
    cbuffer[5] = ( uint8_t ) ( 0xBE >> 0 );
    cbuffer[6] = ( uint8_t ) ( 0xE2 >> 0 );
    cbuffer[7] = ( uint8_t ) ( 0x2E >> 0 );
    cbuffer[8] = ( uint8_t ) ( 0x40 >> 0 );
    cbuffer[9] = ( uint8_t ) ( 0x9F >> 0 );
    cbuffer[10] = ( uint8_t ) ( 0x96 >> 0 );
    cbuffer[11] = ( uint8_t ) ( 0xE9 >> 0 );
    cbuffer[12] = ( uint8_t ) ( 0x3D >> 0 );
    cbuffer[13] = ( uint8_t ) ( 0x7E >> 0 );
    cbuffer[14] = ( uint8_t ) ( 0x11 >> 0 );
    cbuffer[15] = ( uint8_t ) ( 0x73 >> 0 );
    cbuffer[16] = ( uint8_t ) ( 0x93 >> 0 );
    cbuffer[17] = ( uint8_t ) ( 0x17 >> 0 );
    cbuffer[18] = ( uint8_t ) ( 0x2A >> 0 );

    if (lr1110_spi_read( context, cbuffer, 2+1+16, rbuffer, 1+1+16 ) == LR1110_SPI_STATUS_OK) {
        switch (rbuffer[1]) {
        case 0:
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            //HAL_DBG_TRACE_INFO_VALUE("0: CRYP_API_SUCCESS. The previous command was successful\r\n");
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
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData1: 0x%X\r\n", rbuffer[2]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData2: 0x%X\r\n", rbuffer[3]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData3: 0x%X\r\n", rbuffer[4]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData4: 0x%X\r\n", rbuffer[5]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData5: 0x%X\r\n", rbuffer[6]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData6: 0x%X\r\n", rbuffer[7]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData7: 0x%X\r\n", rbuffer[8]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData8: 0x%X\r\n", rbuffer[9]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData9: 0x%X\r\n", rbuffer[10]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData10: 0x%X\r\n", rbuffer[11]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData11: 0x%X\r\n", rbuffer[12]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData12: 0x%X\r\n", rbuffer[13]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData13: 0x%X\r\n", rbuffer[14]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData14: 0x%X\r\n", rbuffer[15]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData15: 0x%X\r\n", rbuffer[16]);
        HAL_DBG_TRACE_INFO_VALUE("EncryptedData16: 0x%X\r\n", rbuffer[17]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get LR1110 encrypted data\r\n");
    }
}

void setLR1110_RF_Frequency( const void* context, const uint32_t rf_frequency) {
    HAL_DBG_TRACE_INFO("Setting LR1110 RF frequency... ");

    uint8_t cbuffer[LR1110_LORA_CMD_LENGTH_SET_RF_FREQUENCY];

    cbuffer[0] = ( uint8_t )( LR1110_LORA_CMD_SET_RF_FREQUENCY >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_LORA_CMD_SET_RF_FREQUENCY >> 0 );
    cbuffer[2] = ( uint8_t )( rf_frequency >> 24 );
    cbuffer[3] = ( uint8_t )( rf_frequency >> 16 );
    cbuffer[4] = ( uint8_t )( rf_frequency >> 8 );
    cbuffer[5] = ( uint8_t )( rf_frequency >> 0 );

    if (lr1110_spi_write( context, cbuffer, LR1110_LORA_CMD_LENGTH_SET_RF_FREQUENCY ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set LR1110 RF frequency\r\n");
    }
}