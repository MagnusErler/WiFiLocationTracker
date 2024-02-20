
#include "gnss.h"

#include "spi.h"

#include "main.h"       // for HAL_DBG_TRACE-functions


lr1110_spi_status_t lr11xx_gnss_get_nb_detected_satellites( const void* context, uint8_t* nb_detected_satellites ) {
  HAL_DBG_TRACE_INFO("Getting number of detected satellites... ");

  uint8_t cbuffer[LR1110_GET_NUMBER_OF_SATELLITES_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_NUMBER_OF_SATELLITES_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_NUMBER_OF_SATELLITES_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_NUMBER_OF_SATELLITES_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_NUMBER_OF_SATELLITES_CMD_LENGTH, rbuffer, LR1110_GET_NUMBER_OF_SATELLITES_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("%d\r\n", (uint8_t)rbuffer[0]);
    return LR1110_SPI_STATUS_OK;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get number of detected satellites\r\n");
    return LR1110_SPI_STATUS_ERROR;
  }
}

lr1110_spi_status_t lr11xx_gnss_scan_autonomous( const void* context ) {
  HAL_DBG_TRACE_INFO("Scanning GNSS satellites... ");

  uint8_t cbuffer[LR1110_SCAN_GNSS_AUTONOMOUS_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_SCAN_GNSS_AUTONOMOUS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SCAN_GNSS_AUTONOMOUS_CMD >> 0 );
  cbuffer[2] = ( uint8_t )( 0x52FFD96B >> 24 );
  cbuffer[3] = ( uint8_t )( 0x52FFD96B >> 16 );
  cbuffer[4] = ( uint8_t )( 0x52FFD96B >> 8 );
  cbuffer[5] = ( uint8_t )( 0x52FFD96B >> 0 );
  cbuffer[6] = ( uint8_t )( 0x00 >> 0 );
  cbuffer[7] = ( uint8_t )( 0x00 >> 0 );
  cbuffer[8] = ( uint8_t )( 0x02 >> 0 );

  if (lr1110_spi_write( context, cbuffer, LR1110_SCAN_GNSS_AUTONOMOUS_CMD_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    return LR1110_SPI_STATUS_OK;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to scan GNSS satellites\r\n");
    return LR1110_SPI_STATUS_ERROR;
  }
}

lr1110_spi_status_t lr11xx_get_gnss_results( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting GNSS results... \r\n");

  uint8_t cbuffer[LR1110_GET_RESULTS_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_RESULTS_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_RESULTS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_RESULTS_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_RESULTS_CMD_LENGTH, rbuffer, LR1110_GET_RESULTS_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO("GNSS results 0: %d\r\n", rbuffer[0]);
    HAL_DBG_TRACE_INFO("GNSS results 1: %d\r\n", rbuffer[1]);
    return LR1110_SPI_STATUS_OK;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get GNSS results\r\n");
    return LR1110_SPI_STATUS_ERROR;
  }
}

lr1110_spi_status_t lr11xx_get_gnss_sv_detected( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting GNSS satellites detected... \r\n");

  uint8_t cbuffer[LR1110_GET_SV_DETECTED_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_SV_DETECTED_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_SV_DETECTED_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_SV_DETECTED_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_SV_DETECTED_CMD_LENGTH, rbuffer, LR1110_GET_SV_DETECTED_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO("GNSS SV detected 0: %d\r\n", rbuffer[0]);
    HAL_DBG_TRACE_INFO("GNSS SV detected 1: %d\r\n", rbuffer[1]);
    return LR1110_SPI_STATUS_OK;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get GNSS satellites detected\r\n");
    return LR1110_SPI_STATUS_ERROR;
  }
}

void scanLR1110_GNSS_Satellites( const void* context ) {

  HAL_GPIO_WritePin(LNA_CTRL_GPIO_Port, LNA_CTRL_Pin, GPIO_PIN_SET);
  
  if (lr11xx_gnss_scan_autonomous( context ) != LR1110_SPI_STATUS_OK) {
    return;
  }

  HAL_Delay( 5000 );

  uint8_t nb_sat;
  if (lr11xx_gnss_get_nb_detected_satellites( context, &nb_sat ) != LR1110_SPI_STATUS_OK ) {
    return;
  }
  if (lr11xx_get_gnss_sv_detected( context ) != LR1110_SPI_STATUS_OK) {
    return;
  }

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