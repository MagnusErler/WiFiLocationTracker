/**
  ******************************************************************************
  * @file           : lr1110.c
  * @brief          : Containing utility functions for LR1110
  ******************************************************************************
  */

#include "lr1110.h"

#include "spi.h"
#include "main.h"       // for HAL_DBG_TRACE-functions

void getLR1110_Bootloader_Version( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 version... ");

  uint8_t cbuffer[LR1110_GET_VERSION_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_VERSION_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_VERSION_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_VERSION_CMD >> 0 );
  
  if (lr1110_spi_read(context, cbuffer, LR1110_GET_VERSION_CMD_LENGTH, rbuffer, LR1110_GET_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("HW: %d (0x%X), ", rbuffer[0], rbuffer[0]);
    HAL_DBG_TRACE_INFO_VALUE("FW: %d.%d (0x%X.0x%X), ", rbuffer[2], rbuffer[3], rbuffer[2], rbuffer[3]);
    switch (rbuffer[1]) {
        case 1:
            HAL_DBG_TRACE_INFO_VALUE("Bootloader type: LR1110 (0x%X)\r\n", rbuffer[1]);
            break;
        case 2:
            HAL_DBG_TRACE_INFO_VALUE("Bootloader type: LR1120 (0x%X)\r\n", rbuffer[1]);
            break;
        case 3:
            HAL_DBG_TRACE_INFO_VALUE("Bootloader type: LR1121 (0x%X)\r\n", rbuffer[1]);
            break;
    }
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 bootloader version\r\n");
  }
}

float getLR1110_Temperature( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 temperature... ");

  uint8_t cbuffer[LR1110_GET_TEMPERATURE_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_TEMPERATURE_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_TEMPERATURE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_TEMPERATURE_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_TEMPERATURE_CMD_LENGTH, rbuffer, LR1110_GET_TEMPERATURE_LENGTH ) == LR1110_SPI_STATUS_OK) {

    uint16_t temp_10_0 = ((rbuffer[0] << 8) | rbuffer[1]) & 0x7FF;
    float temperature = 25 + (1000/(-1.7)) * ((temp_10_0/2047.0) * 1.35 - 0.7295);
    
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

  uint8_t cbuffer[LR1110_GET_CHIPEUI_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_CHIPEUI_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_CHIPEUI_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_CHIPEUI_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_CHIPEUI_CMD_LENGTH, rbuffer, LR1110_GET_CHIPEUI_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", rbuffer[0], rbuffer[1], rbuffer[2], rbuffer[3], rbuffer[4], rbuffer[5], rbuffer[6], rbuffer[7]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 ChipEUI\r\n");
  }
}

void getLR1110_Semtech_JoinEui( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 Semtech JoinEUI... ");

  uint8_t cbuffer[LR1110_GET_SEMTECH_JOINEUI_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_SEMTECH_JOINEUI_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_SEMTECH_JOINEUI_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_SEMTECH_JOINEUI_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_SEMTECH_JOINEUI_CMD_LENGTH, rbuffer, LR1110_GET_SEMTECH_JOINEUI_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_INFO_VALUE("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", rbuffer[0], rbuffer[1], rbuffer[2], rbuffer[3], rbuffer[4], rbuffer[5], rbuffer[6], rbuffer[7]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 Semtech JoinEUI\r\n");
  }
}

float getLR1110_Battery_Voltage( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 battery voltage... ");

  uint8_t cbuffer[LR1110_GET_BATTERY_VOLTAGE_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_BATTERY_VOLTAGE_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_BATTERY_VOLTAGE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_BATTERY_VOLTAGE_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_BATTERY_VOLTAGE_CMD_LENGTH, rbuffer, LR1110_GET_BATTERY_VOLTAGE_LENGTH ) == LR1110_SPI_STATUS_OK) {

    float batteryVoltage = (((5 * rbuffer[0])/255.0) - 1) * 1.35;
    HAL_DBG_TRACE_INFO_VALUE("%d.%d V\r\n", (uint8_t)batteryVoltage, (uint8_t)((batteryVoltage - (uint8_t)batteryVoltage) * 100));
    return batteryVoltage;
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 battery voltage\r\n");
    return -1;
  }
}

void setupLR1110_TCXO( const void* context ) {
  HAL_DBG_TRACE_INFO( "Setting LR1110 TCXO mode... " );

  uint8_t cbuffer[LR1110_SET_TCXO_MODE_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_SET_TCXO_MODE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SET_TCXO_MODE_CMD >> 0 );
  cbuffer[2] = ( uint8_t ) 0x02;

  const uint8_t timeout = ( 5 * 1000 ) / 30.52;  // BOARD_TCXO_WAKEUP_TIME = 5               // 163

  cbuffer[3] = ( uint8_t )( timeout >> 16 );
  cbuffer[4] = ( uint8_t )( timeout >> 8 );
  cbuffer[5] = ( uint8_t )( timeout >> 0 );
  cbuffer[5] = ( uint8_t ) 0xA3;

  if (lr1110_spi_write( context, cbuffer, LR1110_SET_TCXO_MODE_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set LR1110 TCXO mode\r\n");
  }
}

void getStatus( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 status... ");

  uint8_t cbuffer[LR1110_GET_STATUS_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_GET_STATUS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_STATUS_CMD >> 0 );
  cbuffer[2] = 0x00;
  cbuffer[3] = 0x00;
  cbuffer[4] = 0x00;
  cbuffer[5] = 0x00;

  if (lr1110_spi_write( context, cbuffer, LR1110_GET_STATUS_CMD_LENGTH, true ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 status\r\n");
  }
}

void print_binary(uint32_t num) {
    for(int i = 7; i >= 0; i--) {
        HAL_DBG_TRACE_PRINTF("%d", (num >> i) & 1);
    }
    HAL_DBG_TRACE_PRINTF(" ");
}

void calibrateLR1110_Image( const void* context, uint8_t freq1, uint8_t freq2) {
  HAL_DBG_TRACE_INFO("Calibrating LR1110 image... ");

  uint8_t cbuffer[LR1110_CALIBRATE_IMAGE_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_CALIBRATE_IMAGE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CALIBRATE_IMAGE_CMD >> 0 );
  cbuffer[2] = freq1;
  cbuffer[3] = freq2;

  if (lr1110_spi_write( context, cbuffer, LR1110_CALIBRATE_IMAGE_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to calibrate LR1110 image\r\n");
  }
}

void calibrateLR1110( const void* context, uint8_t calib_params) {
  HAL_DBG_TRACE_INFO("Calibrating LR1110... ");

  uint8_t cbuffer[LR1110_CALIBRATE_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_CALIBRATE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CALIBRATE_CMD >> 0 );
  cbuffer[2] = calib_params;

  if (lr1110_spi_write( context, cbuffer, LR1110_CALIBRATE_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to calibrate LR1110\r\n");
  }
}

void clearLR1110_Errors( const void* context) {
  HAL_DBG_TRACE_INFO("Clearing LR1110 errors... ");

  uint8_t cbuffer[LR1110_CLEAR_ERRORS_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_CLEAR_ERRORS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_CLEAR_ERRORS_CMD >> 0 );

  if (lr1110_spi_write( context, cbuffer, LR1110_CLEAR_ERRORS_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to clear LR1110 errors\r\n");
  }
}

void getErrors( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 errors...");

  uint8_t cbuffer[LR1110_GET_ERRORS_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GET_ERRORS_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_ERRORS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_ERRORS_CMD >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_GET_ERRORS_CMD_LENGTH, rbuffer, LR1110_GET_ERRORS_LENGTH ) == LR1110_SPI_STATUS_OK) {
    #define BIT_0 0b00000001
    #define BIT_1 0b00000010
    #define BIT_8 0b00000100
    #define BIT_12 0b00001000
    #define BIT_16 0b00010000
    #define BIT_20 0b00100000
    #define BIT_24 0b01000000
    #define BIT_28 0b10000000

    print_binary(rbuffer[1]);
    print_binary(rbuffer[2]);

    HAL_DBG_TRACE_PRINTF("\r\n");
    if (rbuffer[1] & BIT_0) {
        HAL_DBG_TRACE_PRINTF("LF_RC_CALIB_ERR. Calibration of low frequency RC was not done. To fix it redo a calibration.\r\n");
    }
    if (rbuffer[1] & BIT_1) {
        HAL_DBG_TRACE_PRINTF("HF_RC_CALIB_ERR. Calibration of high frequency RC was not done. To fix it redo a calibration.\r\n");
    }
    if (rbuffer[1] & BIT_8) {
        HAL_DBG_TRACE_PRINTF("ADC_CALIB_ERR. Calibration of ADC was not done. To fix it redo a calibration.\r\n");
    }
    if (rbuffer[1] & BIT_12) {
        HAL_DBG_TRACE_PRINTF("PLL_CALIB_ERR. Calibration of maximum and minimum frequencies was not done. To fix it redo the PLL calibration.\r\n");
    }
    if (rbuffer[1] & BIT_16) {
        HAL_DBG_TRACE_PRINTF("IMG_CALIB_ERR. Calibration of the image rejection was not done. To fix it redo the image calibration.\r\n");
    }
    if (rbuffer[1] & BIT_20) {
        HAL_DBG_TRACE_PRINTF("HF_XOSC_START_ERR. High frequency XOSC did not start correctly. To fix it redo a reset, or send SetTcxoCmd(...) if a TCXO is connected and redo calibrations.\r\n");
    }
    if (rbuffer[1] & BIT_24) {
        HAL_DBG_TRACE_PRINTF("LF_XOSC_START_ERR. Low frequency XOSC did not start correctly. To fix it redo a reset.\r\n");
    }
    if (rbuffer[1] & BIT_28) {
        HAL_DBG_TRACE_PRINTF("PLL_LOCK_ERR. The PLL did not lock. This can come from too high or too low frequency configuration, or if the PLL was not calibrated. To fix it redo a PLL calibration, or use other frequencies.\r\n");
    }

    if (rbuffer[2] & BIT_0) {
        HAL_DBG_TRACE_PRINTF("RX_ADC_OFFSET_ERR. Calibration of ADC offset was not done. To fix it redo a calibration.\r\n");
    }
    if (rbuffer[2] & BIT_1) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (rbuffer[2] & BIT_8) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (rbuffer[2] & BIT_12) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (rbuffer[2] & BIT_16) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (rbuffer[2] & BIT_20) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (rbuffer[2] & BIT_24) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (rbuffer[2] & BIT_28) {
        HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 battery voltage\r\n");
  }
}

void resetLR1110( const void* context, uint8_t reset_type) {
  HAL_DBG_TRACE_INFO("Resetting LR1110... ");
  
  
  radio_t* radio = (radio_t*) context;
  HAL_GPIO_WritePin(radio->reset.port, radio->reset.pin, GPIO_PIN_RESET);
  HAL_Delay(200);   // At least 100ms
  HAL_GPIO_WritePin(radio->reset.port, radio->reset.pin, GPIO_PIN_SET);


  // /////////// DO IT WITH SPI
  // uint8_t cbuffer[LR1110_REBOOT_CMD_LENGTH];

  // cbuffer[0] = ( uint8_t )( LR1110_REBOOT_CMD >> 8 );
  // cbuffer[1] = ( uint8_t )( LR1110_REBOOT_CMD >> 0 );
  // cbuffer[2] = reset_type;

  // if (lr1110_spi_write( context, cbuffer, LR1110_REBOOT_CMD_LENGTH, false ) != LR1110_SPI_STATUS_OK) {
  //   HAL_DBG_TRACE_ERROR("Failed to reset LR1110\r\n");
  //   return;
  // }
  // ///////////

  HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
}

void setLR1110_Dio_Irq_Params( const void* context, uint64_t irq1_to_enable, uint64_t irq2_to_enable) {
  HAL_DBG_TRACE_INFO("Setting LR1110 DIO IRQ parameters... ");

  uint8_t cbuffer[LR1110_SET_DIO_IRQ_PARAMS_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_SET_DIO_IRQ_PARAMS_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SET_DIO_IRQ_PARAMS_CMD >> 0 );
  // Bit 20 is set to 1
  cbuffer[2] = ( uint8_t )( irq1_to_enable >> 24 );
  cbuffer[3] = ( uint8_t )( irq1_to_enable >> 16 );
  cbuffer[4] = ( uint8_t )( irq1_to_enable >> 8 );
  cbuffer[5] = ( uint8_t )( irq1_to_enable >> 0 );

  cbuffer[6] = ( uint8_t )( irq2_to_enable >> 24 );
  cbuffer[7] = ( uint8_t )( irq2_to_enable >> 16 );
  cbuffer[8] = ( uint8_t )( irq2_to_enable >> 8 );
  cbuffer[9] = ( uint8_t )( irq2_to_enable >> 0 );

  if (lr1110_spi_write( context, cbuffer, LR1110_SET_DIO_IRQ_PARAMS_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set LR1110 DIO IRQ parameters\r\n");
  }
}

void setLR1110_Standby_Mode( const void* context, uint8_t stdby_config) {
  HAL_DBG_TRACE_INFO("Setting LR1110 standby mode... ");

  uint8_t cbuffer[LR1110_SET_STANDBY_MODE_CMD_LENGTH];

  cbuffer[0] = ( uint8_t )( LR1110_SET_STANDBY_MODE_CMD >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_SET_STANDBY_MODE_CMD >> 0 );
  cbuffer[2] = stdby_config;

  if (lr1110_spi_write( context, cbuffer, LR1110_SET_STANDBY_MODE_CMD_LENGTH, false ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set LR1110 standby mode\r\n");
  }
}

