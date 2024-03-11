/**
  ******************************************************************************
  * @file           : lr1110.c
  * @brief          : Containing utility functions for LR1110
  ******************************************************************************
  */

#include "lr1110.h"

#include "spi.h"
#include "main.h"       // for HAL_DBG_TRACE-functions
#include "helper.h"

void getLR1110_Bootloader_Version( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 version... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_VERSION];
  uint8_t rbuffer[LR1110_RES_LENGTH_GET_VERSION] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_VERSION_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_VERSION_CMD >> 0 );
  
  if (lr1110_spi_read(context, cbuffer, LR1110_CMD_LENGTH_GET_VERSION, rbuffer, LR1110_RES_LENGTH_GET_VERSION ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("HW: %d (0x%X), ", rbuffer[1], rbuffer[1]);
    HAL_DBG_TRACE_INFO_VALUE("FW: %d.%d (0x%X.0x%X), ", rbuffer[3], rbuffer[4], rbuffer[3], rbuffer[4]);
    switch (rbuffer[2]) {
        case 1:
            HAL_DBG_TRACE_INFO_VALUE("Bootloader type: LR1110 (0x%X)\r\n", rbuffer[2]);
            break;
        case 2:
            HAL_DBG_TRACE_INFO_VALUE("Bootloader type: LR1120 (0x%X)\r\n", rbuffer[2]);
            break;
        case 3:
            HAL_DBG_TRACE_INFO_VALUE("Bootloader type: LR1121 (0x%X)\r\n", rbuffer[2]);
            break;
    }
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 bootloader version\r\n");
  }
}

float getLR1110_Temperature( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 temperature... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_TEMPERATURE];
  uint8_t rbuffer[LR1110_RES_LENGTH_GET_TEMPERATURE] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_TEMPERATURE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_TEMPERATURE_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_CMD_LENGTH_GET_TEMPERATURE, rbuffer, LR1110_RES_LENGTH_GET_TEMPERATURE ) == LR1110_SPI_STATUS_OK) {
    uint16_t temp_10_0 = ((rbuffer[1] << 8) | rbuffer[2]) & 0x7FF;
    const float temperature = 25 + (1000/(-1.7)) * ((temp_10_0/2047.0) * 1.35 - 0.7295);
    
    HAL_DBG_TRACE_INFO_VALUE("%d.%d °C\r\n", (uint8_t)temperature, (uint8_t)((temperature - (uint8_t)temperature) * 100));
    if ((uint8_t)temperature > 50) {
      HAL_DBG_TRACE_ERROR("LR1110 temperature is too high. TCXO mode is maybe not set up correctly\r\n");
    }
    return temperature;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 temperature\r\n");
    return -1;
  }
}

void getLR1110_ChipEUI( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 ChipEUI... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_CHIPEUI];
  uint8_t rbuffer[LR1110_RES_LENGTH_GET_CHIPEUI] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_CHIPEUI_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_CHIPEUI_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_CMD_LENGTH_GET_CHIPEUI, rbuffer, LR1110_RES_LENGTH_GET_CHIPEUI ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", rbuffer[1], rbuffer[2], rbuffer[3], rbuffer[4], rbuffer[5], rbuffer[6], rbuffer[7], rbuffer[8]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 ChipEUI\r\n");
  }
}

void getLR1110_Semtech_JoinEui( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 Semtech JoinEUI... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_SEMTECH_JOINEUI];
  uint8_t rbuffer[LR1110_RES_LENGTH_GET_SEMTECH_JOINEUI] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_SEMTECH_JOINEUI_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_SEMTECH_JOINEUI_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_CMD_LENGTH_GET_SEMTECH_JOINEUI, rbuffer, LR1110_RES_LENGTH_GET_SEMTECH_JOINEUI ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", rbuffer[1], rbuffer[2], rbuffer[3], rbuffer[4], rbuffer[5], rbuffer[6], rbuffer[7], rbuffer[8]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 Semtech JoinEUI\r\n");
  }
}

float getLR1110_Battery_Voltage( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 battery voltage... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_BATTERY_VOLTAGE];
  uint8_t rbuffer[LR1110_RES_LENGTH_GET_BATTERY_VOLTAGE] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_BATTERY_VOLTAGE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_BATTERY_VOLTAGE_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_CMD_LENGTH_GET_BATTERY_VOLTAGE, rbuffer, LR1110_RES_LENGTH_GET_BATTERY_VOLTAGE ) == LR1110_SPI_STATUS_OK) {
    const float batteryVoltage = (((5 * rbuffer[1])/255.0) - 1) * 1.35;
    HAL_DBG_TRACE_INFO_VALUE("%d.%d V\r\n", (uint8_t)batteryVoltage, (uint8_t)((batteryVoltage - (uint8_t)batteryVoltage) * 100));
    return batteryVoltage;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 battery voltage\r\n");
    return -1;
  }
}

void setLR1110_TCXO_Mode( const void* context ) {
  HAL_DBG_TRACE_INFO( "Setting LR1110 TCXO mode... " );

  uint8_t cbuffer[LR1110_CMD_LENGTH_SET_TCXO_MODE];

  cbuffer[0] = ( uint8_t )( LR1110_SET_TCXO_MODE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SET_TCXO_MODE_CMD >> 0 );

  const uint8_t timeout = ( 5 * 1000 ) / 30.52;  // BOARD_TCXO_WAKEUP_TIME = 5  // 163 (163.826998689)
  cbuffer[2] = ( uint8_t ) 0x02;
  cbuffer[3] = ( uint8_t )( timeout >> 16 );
  cbuffer[4] = ( uint8_t )( timeout >> 8 );
  cbuffer[5] = ( uint8_t )( timeout >> 0 );

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_SET_TCXO_MODE ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set LR1110 TCXO mode\r\n");
  }
}

void getStatus( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 status... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_STATUS];

  cbuffer[0] = ( uint8_t )( LR1110_GET_STATUS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_STATUS_CMD >> 0 );
  cbuffer[2] = 0x00;
  cbuffer[3] = 0x00;
  cbuffer[4] = 0x00;
  cbuffer[5] = 0x00;

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_GET_STATUS ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 status\r\n");
  }
}

void calibrateLR1110_Image( const void* context, uint8_t freq1, uint8_t freq2) {
  HAL_DBG_TRACE_INFO("Calibrating LR1110 image... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_CALIBRATE_IMAGE];

  cbuffer[0] = ( uint8_t )( LR1110_CALIBRATE_IMAGE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CALIBRATE_IMAGE_CMD >> 0 );
  cbuffer[2] = freq1;
  cbuffer[3] = freq2;

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_CALIBRATE_IMAGE ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to calibrate LR1110 image\r\n");
  }
}

void calibrateLR1110( const void* context, uint8_t calib_params) {
  HAL_DBG_TRACE_INFO("Calibrating LR1110... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_CALIBRATE];

  cbuffer[0] = ( uint8_t )( LR1110_CALIBRATE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CALIBRATE_CMD >> 0 );
  cbuffer[2] = calib_params;

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_CALIBRATE ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to calibrate LR1110\r\n");
  }
}

void clearLR1110_Errors( const void* context) {
  HAL_DBG_TRACE_INFO("Clearing all pending error flags...");

  uint8_t cbuffer[LR1110_CMD_LENGTH_CLEAR_ERRORS];

  cbuffer[0] = ( uint8_t )( LR1110_CLEAR_ERRORS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CLEAR_ERRORS_CMD >> 0 );

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_CLEAR_ERRORS ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to clear LR1110 errors\r\n");
  }
}

void clearLR1110_IRQ( const void* context) {
  HAL_DBG_TRACE_INFO("Clearing LR1110 interrupts... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_CLEAR_INTERRUPTS];

  cbuffer[0] = ( uint8_t )( LR1110_CLEAR_INTERRUPTS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CLEAR_INTERRUPTS_CMD >> 0 );
  cbuffer[2] = 0b11111111;
  cbuffer[3] = 0b11111111;
  cbuffer[4] = 0b11111111;
  cbuffer[5] = 0b11111111;

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_CLEAR_INTERRUPTS ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to clear LR1110 interrupts\r\n");
  }
}

void getErrors( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 errors...");

  uint8_t cbuffer[LR1110_CMD_LENGTH_GET_ERRORS];
  uint8_t rbuffer[LR1110_RES_LENGTH_GET_ERRORS] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_ERRORS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_ERRORS_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_CMD_LENGTH_GET_ERRORS, rbuffer, LR1110_RES_LENGTH_GET_ERRORS ) == LR1110_SPI_STATUS_OK) {
    // Extracting error status bits
    uint16_t error_status = (rbuffer[1] << 8) | rbuffer[2];

    HAL_DBG_TRACE_MSG_COLOR("\r\nError Status\r\n", HAL_DBG_TRACE_COLOR_YELLOW);

    // Print binary representation of error status
    // TODO: Make a function for this and put it into helpers
    HAL_DBG_TRACE_PRINTF("Error Status: ");
    for (int i = 15; i >= 0; --i) {
        HAL_DBG_TRACE_PRINTF("%d", (error_status >> i) & 0x01);
        if (i % 4 == 0) {
            HAL_DBG_TRACE_PRINTF(" ");
        }
    }
    HAL_DBG_TRACE_PRINTF("(0x%04X)\r\n", error_status);

    // Interpretations
    if (error_status & (1 << 0)) { HAL_DBG_TRACE_PRINTF("LF_RC_CALIB_ERR: Calibration of low frequency RC was not done. To fix it redo a calibration.\r\n"); }
    if (error_status & (1 << 1)) { HAL_DBG_TRACE_PRINTF("HF_RC_CALIB_ERR: Calibration of high frequency RC was not done. To fix it redo a calibration.\r\n"); }
    if (error_status & (1 << 2)) { HAL_DBG_TRACE_PRINTF("ADC_CALIB_ERR: Calibration of ADC was not done. To fix it redo a calibration.\r\n"); }
    if (error_status & (1 << 3)) { HAL_DBG_TRACE_PRINTF("PLL_CALIB_ERR: Calibration of maximum and minimum frequencies was not done. To fix it redo the PLL calibration.\r\n"); }
    if (error_status & (1 << 4)) { HAL_DBG_TRACE_PRINTF("IMG_CALIB_ERR: Calibration of the image rejection was not done. To fix it redo the image calibration.\r\n"); }
    if (error_status & (1 << 5)) { HAL_DBG_TRACE_PRINTF("HF_XOSC_START_ERR: High frequency XOSC did not start correctly. To fix it redo a reset, or send SetTcxoCmd(...) if a TCXO is connected and redo calibrations.\r\n"); }
    if (error_status & (1 << 6)) { HAL_DBG_TRACE_PRINTF("LF_XOSC_START_ERR: Low frequency XOSC did not start correctly. To fix it redo a reset.\r\n"); }
    if (error_status & (1 << 7)) { HAL_DBG_TRACE_PRINTF("PLL_LOCK_ERR: The PLL did not lock. This can come from too high or too low frequency configuration, or if the PLL was not calibrated. To fix it redo a PLL calibration, or use other frequencies.\r\n"); }
    if (error_status & (1 << 8)) { HAL_DBG_TRACE_PRINTF("RX_ADC_OFFSET_ERR: Calibration of ADC offset was not done. To fix it redo a calibration.\r\n"); }
    if (error_status & 0xFF00)   { HAL_DBG_TRACE_PRINTF("RFU\r\n"); }
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 errors\r\n");
  }
}

void resetLR1110( const void* context, const uint8_t reset_type) {
  HAL_DBG_TRACE_INFO("Resetting LR1110... ");
  
  
  radio_t* radio = (radio_t*) context;
  HAL_GPIO_WritePin(radio->reset.port, radio->reset.pin, GPIO_PIN_RESET);
  HAL_Delay(200);   // At least 100ms
  HAL_GPIO_WritePin(radio->reset.port, radio->reset.pin, GPIO_PIN_SET);


  // /////////// DO IT WITH SPI
  // REMEMBER THAT BUSY STAYS HIGH WHILE THE CHIP IS REBOOTING
  // uint8_t cbuffer[LR1110_CMD_LENGTH_REBOOT];

  // cbuffer[0] = ( uint8_t )( LR1110_REBOOT_CMD >> 8 );
  // cbuffer[1] = ( uint8_t )( LR1110_REBOOT_CMD >> 0 );
  // cbuffer[2] = reset_type;

  // if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_REBOOT, true ) != LR1110_SPI_STATUS_OK) {
  //   HAL_DBG_TRACE_ERROR("Failed to reset LR1110\r\n");
  //   return;
  // }
  // ///////////

  HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
}

void setLR1110_Dio_Irq_Params( const void* context, const uint64_t irq1_to_enable, const uint64_t irq2_to_enable) {
  HAL_DBG_TRACE_INFO("Setting LR1110 DIO IRQ parameters... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_SET_DIO_IRQ_PARAMS];

  cbuffer[0] = ( uint8_t )( LR1110_SET_DIO_IRQ_PARAMS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SET_DIO_IRQ_PARAMS_CMD >> 0 );

  cbuffer[2] = ( uint8_t )( irq1_to_enable >> 24 );
  cbuffer[3] = ( uint8_t )( irq1_to_enable >> 16 );
  cbuffer[4] = ( uint8_t )( irq1_to_enable >> 8 );
  cbuffer[5] = ( uint8_t )( irq1_to_enable >> 0 );

  cbuffer[6] = ( uint8_t )( irq2_to_enable >> 24 );
  cbuffer[7] = ( uint8_t )( irq2_to_enable >> 16 );
  cbuffer[8] = ( uint8_t )( irq2_to_enable >> 8 );
  cbuffer[9] = ( uint8_t )( irq2_to_enable >> 0 );

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_SET_DIO_IRQ_PARAMS ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set LR1110 DIO IRQ parameters\r\n");
  }
}

void setLR1110_Standby_Mode( const void* context, const uint8_t stdby_config) {
  HAL_DBG_TRACE_INFO("Setting LR1110 standby mode... ");

  uint8_t cbuffer[LR1110_CMD_LENGTH_SET_STANDBY_MODE];

  cbuffer[0] = ( uint8_t )( LR1110_SET_STANDBY_MODE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SET_STANDBY_MODE_CMD >> 0 );
  cbuffer[2] = stdby_config;

  if (lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_SET_STANDBY_MODE ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set LR1110 standby mode\r\n");
  }
}

void clearLR1110_RX_Buffer( const void* context) {
    HAL_DBG_TRACE_INFO("Clearing RX buffer... ");

    uint8_t cbuffer[LR1110_CMD_LENGTH_CLEAR_RX_BUFFER];

    cbuffer[0] = ( uint8_t )( LR1110_CLEAR_RX_BUFFER >> 8 );
    cbuffer[1] = ( uint8_t )( LR1110_CLEAR_RX_BUFFER >> 0 );

    if ( lr1110_spi_write( context, cbuffer, LR1110_CMD_LENGTH_CLEAR_RX_BUFFER ) == LR1110_SPI_STATUS_OK ) {
        HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
    } else {
        HAL_DBG_TRACE_ERROR("Failed to clear RX buffer\r\n");
    }
}