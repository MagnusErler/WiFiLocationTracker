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

    uint8_t cbuffer[LR1110_GET_NUMBER_OF_SATELLITES_CMD_LENGTH];
    uint8_t rbuffer[LR1110_GET_NUMBER_OF_SATELLITES_LENGTH] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GET_NUMBER_OF_SATELLITES_CMD >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GET_NUMBER_OF_SATELLITES_CMD >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GET_NUMBER_OF_SATELLITES_CMD_LENGTH, rbuffer, LR1110_GET_NUMBER_OF_SATELLITES_LENGTH ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_INFO_VALUE("%d\r\n", (uint8_t)rbuffer[0]);
        return (uint8_t)rbuffer[0];
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get number of detected satellites\r\n");
        return 0;
    }
}

void getLR1110_GNSS_Detected_Satellites( const void* context, uint8_t nb_of_satellites) {
    HAL_DBG_TRACE_INFO("Getting GNSS satellites detected... \r\n");

    uint8_t cbuffer[LR1110_GET_SATELLITES_DETECTED_CMD_LENGTH];
    uint8_t rbuffer[LR1110_GET_SATELLITES_DETECTED_LENGTH + nb_of_satellites];

    cbuffer[0] = ( uint8_t )( LR1110_GET_SATELLITES_DETECTED_CMD >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GET_SATELLITES_DETECTED_CMD >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GET_SATELLITES_DETECTED_CMD_LENGTH, rbuffer, LR1110_GET_SATELLITES_DETECTED_LENGTH + nb_of_satellites ) == LR1110_SPI_STATUS_OK) {
        for (uint8_t i = 0; i < nb_of_satellites; i++) {
            HAL_DBG_TRACE_INFO_VALUE("Sattelite ID %d: %d, C/N0 %d\r\n", i, rbuffer[i], rbuffer[i+1]);
        }
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS satellites detected\r\n");
    }
}

void scanLR1110_GNSS_Satellites( const void* context, uint8_t effort_mode, uint8_t result_mask, uint8_t nb_sv_max ) {
    HAL_DBG_TRACE_INFO("Scanning GNSS satellites... ");

    HAL_GPIO_WritePin(LNA_CTRL_GPIO_Port, LNA_CTRL_Pin, GPIO_PIN_SET);
    turnOnLED(SNIFFING_LED_GPIO_Port, SNIFFING_LED_Pin);

    uint8_t cbuffer[LR1110_SCAN_GNSS_AUTONOMOUS_CMD_LENGTH];

    cbuffer[0] = ( uint8_t )( LR1110_SCAN_GNSS_AUTONOMOUS_CMD >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_SCAN_GNSS_AUTONOMOUS_CMD >> 0 );
    cbuffer[2] = ( uint8_t )( 0x52FFD96B >> 24 );
    cbuffer[3] = ( uint8_t )( 0x52FFD96B >> 16 );
    cbuffer[4] = ( uint8_t )( 0x52FFD96B >> 8 );
    cbuffer[5] = ( uint8_t )( 0x52FFD96B >> 0 );
    cbuffer[6] = ( uint8_t )( effort_mode >> 0 );
    cbuffer[7] = ( uint8_t )( result_mask >> 0 );
    cbuffer[8] = ( uint8_t )( nb_sv_max >> 0 );

    if (lr1110_spi_write( context, cbuffer, LR1110_SCAN_GNSS_AUTONOMOUS_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
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

    uint8_t cbuffer[LR1110_GET_GNSS_VERSION_CMD_LENGTH];
    uint8_t rbuffer[LR1110_GET_GNSS_VERSION_LENGTH] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GET_GNSS_VERSION_CMD >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GET_GNSS_VERSION_CMD >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GET_GNSS_VERSION_CMD_LENGTH, rbuffer, LR1110_GET_GNSS_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_INFO_VALUE("firmware version: %d, ", rbuffer[0]);
        HAL_DBG_TRACE_INFO_VALUE("almanac version: %d\n\r", rbuffer[1]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS version\r\n");
    }
}

void getLR1110_GNSS_Consumption( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting GNSS consumption...  \r\n");

    uint8_t cbuffer[LR1110_GET_CONSUMPTION_CMD_LENGTH];
    uint8_t rbuffer[LR1110_GET_CONSUMPTION_LENGTH] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GET_CONSUMPTION_CMD >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GET_CONSUMPTION_CMD >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GET_CONSUMPTION_CMD_LENGTH, rbuffer, LR1110_GET_CONSUMPTION_LENGTH ) == LR1110_SPI_STATUS_OK) {
        uint32_t cpu_time_microseconds = (uint32_t)(rbuffer[0]) << 24 |
                                         (uint32_t)(rbuffer[1]) << 16 |
                                         (uint32_t)(rbuffer[2]) << 8 |
                                         (uint32_t)(rbuffer[3]);
        uint32_t radio_time_microseconds = (uint32_t)(rbuffer[4]) << 24 |
                                           (uint32_t)(rbuffer[5]) << 16 |
                                           (uint32_t)(rbuffer[6]) << 8 |
                                           (uint32_t)(rbuffer[7]);

        // Convert microseconds to seconds
        float cpu_time_seconds = cpu_time_microseconds / 1000000.0;
        float radio_time_seconds = radio_time_microseconds / 1000000.0;

        HAL_DBG_TRACE_INFO_VALUE("GNSS consumption CPU time: %d.%d seconds\r\n", (uint8_t)cpu_time_seconds, (uint8_t)((cpu_time_seconds - (uint8_t)cpu_time_seconds) * 1000));
        HAL_DBG_TRACE_INFO_VALUE("GNSS consumption radio time: %d.%d seconds\r\n", (uint8_t)radio_time_seconds, (uint8_t)((radio_time_seconds - (uint8_t)radio_time_seconds) * 1000));

    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS consumption\r\n");
    }
}

void getLR1110_GNSS_Almanac_Status( const void* context ) {
    HAL_DBG_TRACE_INFO("Getting GNSS Almanac status... ");

    uint8_t cbuffer[LR1110_GET_GNSS_ALMANAC_STATUS_CMD_LENGTH];
    uint8_t rbuffer[LR1110_GET_GNSS_ALMANAC_STATUS_LENGTH] = { 0 };

    cbuffer[0] = ( uint8_t )( LR1110_GET_GNSS_ALMANAC_STATUS_CMD >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_GET_GNSS_ALMANAC_STATUS_CMD >> 0 );

    if (lr1110_spi_read( context, cbuffer, LR1110_GET_GNSS_ALMANAC_STATUS_CMD_LENGTH, rbuffer, LR1110_GET_GNSS_ALMANAC_STATUS_LENGTH ) == LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_INFO_VALUE("rbuffer[0]: %d\r\n", rbuffer[0]);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to get GNSS Almanac status\r\n");
    }
}
