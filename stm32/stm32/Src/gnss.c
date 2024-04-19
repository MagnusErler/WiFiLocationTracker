/**
  ******************************************************************************
  * @file           : gnss.c
  * @brief          : Containing all GNSS functions
  ******************************************************************************
  */

#include "gnss.h"

#include "spi.h"
#include "led.h"
#include "main.h"       // for HAL_DBG_TRACE-functions

uint8_t getLR1110_GNSS_Number_of_Detected_Satellites( const void* context) {
    HAL_DBG_TRACE_INFO("Getting number of detected satellites... ");

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_GET_NUMBER_OF_SATELLITES];
    uint8_t rbuffer[LR1110_GNSS_RES_LENGTH_GET_NUMBER_OF_SATELLITES] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_GET_NUMBER_OF_SATELLITES >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_GET_NUMBER_OF_SATELLITES >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_CMD_LENGTH_GET_NUMBER_OF_SATELLITES, rbuffer, LR1110_GNSS_RES_LENGTH_GET_NUMBER_OF_SATELLITES ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_INFO_VALUE("%d\r\n", (uint8_t)rbuffer[1]);
        return (uint8_t)rbuffer[1];
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get number of detected satellites\r\n");
        return 0;
    }
}

void getLR1110_GNSS_Detected_Satellites( const void* context, const uint8_t nb_of_satellites) {
    HAL_DBG_TRACE_INFO("Getting GNSS satellites detected... \r\n");

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_GET_SATELLITES_DETECTED];
    uint8_t rbuffer[LR1110_GNSS_RES_LENGTH_GET_SATELLITES_DETECTED + nb_of_satellites];

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_GET_SATELLITES_DETECTED >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_GET_SATELLITES_DETECTED >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_CMD_LENGTH_GET_SATELLITES_DETECTED, rbuffer, LR1110_GNSS_RES_LENGTH_GET_SATELLITES_DETECTED + nb_of_satellites ) == LR1110_SPI_STATUS_OK) {
        for (uint8_t i = 1; i < nb_of_satellites; i++) {
            HAL_DBG_TRACE_INFO_VALUE("Sattelite ID %d: %d, C/N0 %d\r\n", i, rbuffer[i], rbuffer[i+1]);
        }
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS satellites detected\r\n");
    }
}

void scanLR1110_GNSS_Satellites( const void* context, const uint8_t effort_mode, const uint8_t result_mask, const uint8_t nb_sv_max ) {
    HAL_DBG_TRACE_INFO("Scanning GNSS satellites... ");

    HAL_GPIO_WritePin(LNA_CTRL_GPIO_Port, LNA_CTRL_Pin, GPIO_PIN_SET);
    turnOnLED(SNIFFING_LED_GPIO_Port, SNIFFING_LED_Pin);

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_SCAN_GNSS_AUTONOMOUS];

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_SCAN_GNSS_AUTONOMOUS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_SCAN_GNSS_AUTONOMOUS >> 0 );
    cbuffer[2] = ( uint8_t )( 0x52FFD96B >> 24 );
    cbuffer[3] = ( uint8_t )( 0x52FFD96B >> 16 );
    cbuffer[4] = ( uint8_t )( 0x52FFD96B >> 8 );
    cbuffer[5] = ( uint8_t )( 0x52FFD96B >> 0 );
    cbuffer[6] = ( uint8_t ) effort_mode;
    cbuffer[7] = ( uint8_t ) result_mask;
    cbuffer[8] = ( uint8_t ) nb_sv_max;

    if (lr1110_spi_write( context, cbuffer, LR1110_GNSS_CMD_LENGTH_SCAN_GNSS_AUTONOMOUS ) == LR1110_SPI_STATUS_OK) {
        //HAL_Delay( 5000 );
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        //HAL_Delay( 5000 );
        HAL_DBG_TRACE_ERROR("Failed to scan GNSS satellites\r\n");
    }

    turnOffLED(SNIFFING_LED_GPIO_Port, SNIFFING_LED_Pin);
    HAL_GPIO_WritePin(LNA_CTRL_GPIO_Port, LNA_CTRL_Pin, GPIO_PIN_RESET);
}

void getLR1110_GNSS_Version( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting GNSS version... ");

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_GET_GNSS_VERSION];
    uint8_t rbuffer[LR1110_GNSS_RES_LENGTH_GET_GNSS_VERSION] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_GET_GNSS_VERSION >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_GET_GNSS_VERSION >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_CMD_LENGTH_GET_GNSS_VERSION, rbuffer, LR1110_GNSS_RES_LENGTH_GET_GNSS_VERSION ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_INFO_VALUE("FW: %d (0x%X), ", rbuffer[1], rbuffer[1]);
        HAL_DBG_TRACE_INFO_VALUE("almanac version: %d (0x%X)\r\n", rbuffer[2], rbuffer[2]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS version\r\n");
    }
}

void getLR1110_GNSS_Consumption( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting GNSS consumption...  \r\n");

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_GET_CONSUMPTION];
    uint8_t rbuffer[LR1110_GNSS_RES_LENGTH_GET_CONSUMPTION] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_GET_CONSUMPTION >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_GET_CONSUMPTION >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_CMD_LENGTH_GET_CONSUMPTION, rbuffer, LR1110_GNSS_RES_LENGTH_GET_CONSUMPTION ) == LR1110_SPI_STATUS_OK) {
        const uint32_t cpu_time_microseconds = (uint32_t)(rbuffer[1]) << 24 |
                                         (uint32_t)(rbuffer[2]) << 16 |
                                         (uint32_t)(rbuffer[3]) << 8 |
                                         (uint32_t)(rbuffer[4]);
        const uint32_t radio_time_microseconds = (uint32_t)(rbuffer[5]) << 24 |
                                           (uint32_t)(rbuffer[6]) << 16 |
                                           (uint32_t)(rbuffer[7]) << 8 |
                                           (uint32_t)(rbuffer[8]);

        // Convert microseconds to seconds
        const float cpu_time_seconds = cpu_time_microseconds / 1000000.0;
        const float radio_time_seconds = radio_time_microseconds / 1000000.0;

        HAL_DBG_TRACE_INFO_VALUE("GNSS consumption CPU time: %d.%d seconds\r\n", (uint8_t)cpu_time_seconds, (uint8_t)((cpu_time_seconds - (uint8_t)cpu_time_seconds) * 1000));
        HAL_DBG_TRACE_INFO_VALUE("GNSS consumption radio time: %d.%d seconds\r\n", (uint8_t)radio_time_seconds, (uint8_t)((radio_time_seconds - (uint8_t)radio_time_seconds) * 1000));

    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS consumption\r\n");
    }
}

void getLR1110_GNSS_Almanac_Status( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting GNSS Almanac status... ");

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_GET_GNSS_ALMANAC_STATUS];
    uint8_t rbuffer[LR1110_GNSS_RES_LENGTH_GET_GNSS_ALMANAC_STATUS] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_GET_GNSS_ALMANAC_STATUS >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_GET_GNSS_ALMANAC_STATUS >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_CMD_LENGTH_GET_GNSS_ALMANAC_STATUS, rbuffer, LR1110_GNSS_RES_LENGTH_GET_GNSS_ALMANAC_STATUS ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_INFO_VALUE("rbuffer[0]: %d\r\n", rbuffer[1]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS Almanac status\r\n");
    }
}

void setLR1110_GNSS_Constellation( const void* context, const uint8_t constellation ) {
    HAL_DBG_TRACE_INFO("Setting GNSS constellation... ");

    uint8_t cbuffer[LR1110_GNSS_CMD_LENGTH_SET_CONSTELLATION];

    cbuffer[0] = ( uint8_t )( LR1110_GNSS_CMD_SET_CONSTELLATION >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GNSS_CMD_SET_CONSTELLATION >> 0 );
    cbuffer[2] = ( uint8_t )( constellation >> 0 );

    if (lr1110_spi_write( context, cbuffer, LR1110_GNSS_CMD_LENGTH_SET_CONSTELLATION ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set GNSS constellation\r\n");
    }
}
