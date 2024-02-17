/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "spi.h"

#include "smtc_hal.h"

#include <stdlib.h>   // used for malloc function

//#include "wifi_scan.h"
//#include "gnss_scan.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
LPTIM_HandleTypeDef hlptim1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/*!
 * @brief Get LR1110 version
 *
 * @param [in] context Radio abstraction
 */
static void getLR1110_Bootloader_Version();

/*!
 * @brief Get LR1110 temperature
 *
 * @param [in] context Radio abstraction
 */
static void getLR1110_Temperature( const void* context);

/*!
 * @brief Get LR1110 GNSS version
 *
 * @param [in] context Radio abstraction
 */
static void getLR1110_GNSS_Version( const void* context);

/*!
 * @brief Get LR1110 Wi-Fi version
 *
 * @param [in] context Radio abstraction
 */
static void getLR1110_WiFi_Version( const void* context);

/*!
 * @brief Get LR1110 Chip EUI
 *
 * @param [in] context Radio abstraction
 */
static void getLR1110_Chip_EUI( const void* context);

/*!
 * @brief Reset LR1110
 */
static void resetLR1110();

/*!
 * @brief Setup TCXO
 *
 * @param [in] context Radio abstraction
 */
static void setupTCXO( const void* context );

/*!
 * @brief Turn on LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 */
void turnOnLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin);

/*!
 * @brief Turn off LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 */
void turnOffLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin);

/*!
 * @brief Toggle LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 */
void toggleLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin);

/*!
 * @brief Blink LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 * @param [in] period
 * @param [in] count
 * @param [in] start
 */
void blinkLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin, uint32_t period, uint8_t count, bool start);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Wi-Fi settings */
  // wifi_settings_t               wifi_settings;
  // static wifi_scan_all_results_t capture_result;

  // /* GNSS settings */
  // gnss_settings_t               gnss_settings;
  // uint32_t                      unix_time = 0;
  // gnss_scan_single_result_t     capture_result;
  

  int a = 0;

  void* lr1110_context = (void*) malloc(sizeof(radio_t1));
  ((radio_t1*)lr1110_context)->spi             = SPI1;
  ((radio_t1*)lr1110_context)->nss.port        = GPIOA;
  ((radio_t1*)lr1110_context)->nss.pin         = NSS_Pin;
  ((radio_t1*)lr1110_context)->reset.port      = GPIOA;
  ((radio_t1*)lr1110_context)->reset.pin       = RESET_Pin;
  // ((radio_t1*)lr1110_context)->event.irq1.port = GPIOB;
  // ((radio_t1*)lr1110_context)->event.irq1.pin = EVENT_Pin;
  //((radio_t1*)lr1110_context)->event.pin       = EVENT_Pin;
  //((radio_t1*)lr1110_context)->event.callback  = radio_event_callback;
  ((radio_t1*)lr1110_context)->event.context   = ( ( radio_t1* ) lr1110_context );
  ((radio_t1*)lr1110_context)->busy.port       = GPIOB;
  ((radio_t1*)lr1110_context)->busy.pin        = BUSY_Pin;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_LPTIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  hal_uart_init( HAL_PRINTF_UART_ID, UART_TX, UART_RX );

  HAL_DBG_TRACE_MSG("-----------------------------\r\n\r\n");
  resetLR1110();

  blinkLED(GPIOC, RX_LED_Pin|TX_LED_Pin, 100, 5, true);

  setupTCXO(lr1110_context);

  getLR1110_Bootloader_Version(lr1110_context);   // First call to this function is always returning 0.0 no matter if calling for bootloader og wifi version
  getLR1110_Bootloader_Version(lr1110_context);
  getLR1110_WiFi_Version(lr1110_context);
  getLR1110_Chip_EUI(lr1110_context);

  // /* Init LR1110 modem-e event for WiFi*/
  // lr1110_modem_event_callback.wifi_scan_done = lr1110_modem_wifi_scan_done;
  // lr1110_modem_event_callback.reset          = lr1110_modem_reset_event;
  // modem_response_code                        = lr1110_modem_board_init( lr1110_context, &lr1110_modem_event_callback );
  // if( modem_response_code != LR1110_MODEM_RESPONSE_CODE_OK )
  // {
  //     HAL_DBG_TRACE_ERROR( "lr1110_modem_board_init failed (%d)\r\n", modem_response_code );
  // }

  // /* Init LR1110 modem-e event for GNSS*/
  // memset( &lr1110_modem_event_callback, 0, sizeof( lr1110_modem_event_callback ) );
  // lr1110_modem_event_callback.gnss_scan_done = lr1110_modem_gnss_scan_done;
  // lr1110_modem_event_callback.reset          = lr1110_reset_event;
  // modem_response_code                        = lr1110_modem_board_init( lr1110_context, &lr1110_modem_event_callback );
  // if( modem_response_code != LR1110_MODEM_RESPONSE_CODE_OK ) {
  //     HAL_DBG_TRACE_ERROR( "lr1110_modem_board_init failed (%d)\r\n", modem_response_code );
  // }

  // /* Wi-Fi Parameters */
  // wifi_settings.enabled       = true;
  // wifi_settings.channels      = 0x3FFF;  // by default enable all channels
  // wifi_settings.types         = 0x04;
  // wifi_settings.scan_mode     = 2;
  // wifi_settings.nbr_retrials  = 5;
  // wifi_settings.max_results   = 1;
  // wifi_settings.timeout       = 90;
  // wifi_settings.result_format = LR1110_MODEM_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL;

  /* GNSS Parameters */
  // memset( &gnss_settings, 0, sizeof( gnss_settings ) );
  // gnss_settings.enabled              = true;
  // gnss_settings.constellation_to_use = ( LR1110_MODEM_GNSS_GPS_MASK | LR1110_MODEM_GNSS_BEIDOU_MASK );
  // gnss_settings.scan_type            = AUTONOMOUS_MODE;
  // gnss_settings.search_mode          = LR1110_MODEM_GNSS_OPTION_BEST_EFFORT;

  // if( gnss_settings.scan_type == ASSISTED_MODE ) {
  //     /* Set approximate position for assisted mode */
  //     gnss_settings.assistance_position.latitude  = 55.867;
  //     gnss_settings.assistance_position.longitude = 12.386;

  //     modem_response_code = lr1110_modem_gnss_set_assistance_position( lr1110_context, &gnss_settings.assistance_position );
  //     if( modem_response_code != LR1110_MODEM_RESPONSE_CODE_OK ) {
  //         HAL_DBG_TRACE_ERROR( "lr1110_modem_gnss_set_assistance_position failed (%d)\r\n", modem_response_code );
  //     }

  //     /* Get Unix time from user */
  //     unix_time = 1707491137;

  //     modem_response_code = lr1110_modem_set_gps_time( lr1110_context, unix_time - GNSS_EPOCH_SECONDS + GNSS_LEAP_SECONDS_OFFSET );
  //     if( modem_response_code != LR1110_MODEM_RESPONSE_CODE_OK ) {
  //         HAL_DBG_TRACE_ERROR( "lr1110_modem_set_gps_time failed (%d)\r\n", modem_response_code );
  //     }
  // }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    
    //turnOnLED(GPIOC, RX_LED_Pin);
    //turnOffLED(GPIOC, RX_LED_Pin);
    //toggleLED(GPIOC, RX_LED_Pin|TX_LED_Pin);


    HAL_DBG_TRACE_PRINTF("2a = %d\r\n", a++);

    getLR1110_Bootloader_Version(lr1110_context);

    getLR1110_Temperature(lr1110_context);

    // if( wifi_execute_scan( lr1110_context, &wifi_settings, &capture_result ) == WIFI_SCAN_SUCCESS ) {
    //   HAL_DBG_TRACE_MSG( "Success\n\r" );
    //   HAL_DBG_TRACE_MSG("capture_result = {\n\r");
    //   // HAL_DBG_TRACE_MSG("  scan_mode = %d\n\r", capture_result.scan_mode);
    //   // HAL_DBG_TRACE_MSG("  result_format = %d\n\r", capture_result.result_format);
    //   // HAL_DBG_TRACE_MSG("  nbr_results = %d\n\r", capture_result.nbr_results);
    //   HAL_DBG_TRACE_MSG("  basic_mac_type_channel_results = {\n\r");
    //   HAL_DBG_TRACE_PRINTF("    mac_address = %02X:%02X:%02X:%02X:%02X:%02X\n\r", capture_result.basic_mac_type_channel_results[0].mac_address[0], capture_result.basic_mac_type_channel_results[0].mac_address[1], capture_result.basic_mac_type_channel_results[0].mac_address[2], capture_result.basic_mac_type_channel_results[0].mac_address[3], capture_result.basic_mac_type_channel_results[0].mac_address[4], capture_result.basic_mac_type_channel_results[0].mac_address[5]);
    // } else {
    //   HAL_DBG_TRACE_ERROR( "Wi-Fi scan timeout\n\r" );
    // }

    // gnss_scan_result_t scan_result;
    // memset( &capture_result, 0, sizeof( capture_result ) );
    // scan_result = gnss_scan_execute( lr1110_context, &gnss_settings, &capture_result );
    // if( scan_result == GNSS_SCAN_SUCCESS ) {
    //   HAL_DBG_TRACE_MSG("capture_result = {\n\r");
    //   HAL_DBG_TRACE_PRINTF("  is_valid_nav_message = %d\n\r", capture_result.is_valid_nav_message);
    //   HAL_DBG_TRACE_PRINTF("  nav_message_size = %d\n\r", capture_result.nav_message_size);
    //   HAL_DBG_TRACE_PRINTF("  nav_message = %02X:%02X:%02X:%02X:%02X:%02X\n\r", capture_result.nav_message[0], capture_result.nav_message[1], capture_result.nav_message[2], capture_result.nav_message[3], capture_result.nav_message[4], capture_result.nav_message[5]);
    //   HAL_DBG_TRACE_PRINTF("  nb_detected_satellites = %d\n\r", capture_result.nb_detected_satellites);
    //   HAL_DBG_TRACE_PRINTF("  detected_satellites = {\n\r");
    //   HAL_DBG_TRACE_PRINTF("    satellite_id = %d\n\r", capture_result.detected_satellites[0].satellite_id);
    // } else if (scan_result == GNSS_SCAN_NO_TIME) {
    //   HAL_DBG_TRACE_ERROR( "No time\n\r" );
    // } else if (scan_result == GNSS_SCAN_FAIL) {
    //   HAL_DBG_TRACE_ERROR( "gnss_scan_execute failed (%d)\r\n", scan_result );
    // }
    

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV16;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  __HAL_SPI_ENABLE( &hspi1 );

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RX_LED_Pin|TX_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RESET_Pin|NSS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RX_LED_Pin TX_LED_Pin */
  GPIO_InitStruct.Pin = RX_LED_Pin|TX_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RESET_Pin NSS_Pin */
  GPIO_InitStruct.Pin = RESET_Pin|NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUSY_Pin */
  GPIO_InitStruct.Pin = BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EVENT_Pin */
  GPIO_InitStruct.Pin = EVENT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EVENT_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

void turnOnLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin) {
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

void turnOffLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin) {
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

void toggleLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin) {
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void blinkLED(GPIO_TypeDef* LED_GPIO_Port, uint16_t LED_Pin, uint32_t period, uint8_t count, bool start) {
  if (start) {
    for (uint8_t i = 0; i < count; i++) {
      turnOnLED(LED_GPIO_Port, LED_Pin);
      HAL_Delay(period);
      turnOffLED(LED_GPIO_Port, LED_Pin);
      HAL_Delay(period);
    }
  }
}

void getLR1110_Bootloader_Version( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 version... ");

  uint8_t cbuffer[LR1110_VERSION_CMD_LENGTH];
  uint8_t rbuffer[LR1110_VERSION_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_VERSION_OC >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_VERSION_OC >> 0 );
  
  if (lr1110_spi_read(context, cbuffer, LR1110_VERSION_CMD_LENGTH, rbuffer, LR1110_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

    HAL_DBG_TRACE_INFO("LR1110 bootloader hardware version: %d\r\n", rbuffer[0]);
    switch (rbuffer[1]) {
        case 1:
            HAL_DBG_TRACE_INFO("LR1110 bootloader type: LR1110\r\n");
            break;
        case 2:
            HAL_DBG_TRACE_INFO("LR1110 bootloader type: LR1120\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_INFO("LR1110 bootloader type: LR1121\r\n");
            break;
        default:
            break;
    }
    HAL_DBG_TRACE_INFO("LR1110 bootloader firmware version: %d.%d\r\n", rbuffer[2], rbuffer[3]);
  } else {
    HAL_DBG_TRACE_ERROR("\r\nFailed to get LR1110 bootloader version\r\n");
  }
}

void getLR1110_Temperature( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 temperature... ");

  uint8_t cbuffer[LR1110_TEMPERATURE_CMD_LENGTH];
  uint8_t rbuffer[LR1110_TEMPERATURE_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_TEMPERATURE >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_TEMPERATURE >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_TEMPERATURE_CMD_LENGTH, rbuffer, LR1110_TEMPERATURE_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

    uint16_t temp_10_0 = ((rbuffer[0] << 8) | rbuffer[1]) & 0x7FF;
    float temperature = 25 + (1000/(-1.7)) * ((temp_10_0/2047.0) * 1.35 - 0.7295);

    if ((int)temperature > 50) {
      HAL_DBG_TRACE_ERROR("LR1110 temperature is too high\r\n");
      HAL_DBG_TRACE_ERROR("TCXO mode is maybe not set up correctly\r\n");
    }

    HAL_DBG_TRACE_INFO("LR1110 temperature: %d.%d °C\r\n", (int)temperature, (int)((temperature - (int)temperature) * 100));
  } else {
    HAL_DBG_TRACE_ERROR("\r\nFailed to get LR1110 temperature\r\n");
  }
}

void getLR1110_GNSS_Version( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting GNSS version... ");

  uint8_t cbuffer[LR1110_GNSS_VERSION_CMD_LENGTH];
  uint8_t rbuffer[LR1110_GNSS_VERSION_LENGTH] = { 0 };

  cbuffer[0] = LR1110_GROUP_ID_GNSS;
  cbuffer[1] = LR1110_GNSS_READ_FW_VERSION_CMD;

  if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_VERSION_CMD_LENGTH, rbuffer, LR1110_GNSS_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

    HAL_DBG_TRACE_INFO("GNSS firmware version = %d\n\r", rbuffer[0]);
    HAL_DBG_TRACE_INFO("GNSS almanac version = %d\n\r", rbuffer[1]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get GNSS version\r\n");
  }
}

void getLR1110_WiFi_Version( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting Wi-Fi version... ");

  uint8_t cbuffer[LR1110_WIFI_VERSION_CMD_LENGTH];
  uint8_t rbuffer[LR1110_WIFI_VERSION_LENGTH] = { 0 };

  cbuffer[0] = LR1110_GROUP_ID_WIFI;
  cbuffer[1] = LR1110_WIFI_GET_FIRMWARE_WIFI_VERSION_CMD;

  if (lr1110_spi_read( context, cbuffer, LR1110_WIFI_VERSION_CMD_LENGTH, rbuffer, LR1110_WIFI_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

    HAL_DBG_TRACE_INFO("Wi-Fi firmware version: %d.%d\r\n", rbuffer[0], rbuffer[1]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get Wi-Fi version\r\n");
  }
}

void getLR1110_Chip_EUI( const void* context ) {
  HAL_DBG_TRACE_INFO("Getting LR1110 Chip EUI... ");

  uint8_t cbuffer[LR1110_CHIP_EUI_CMD_LENGTH];
  uint8_t rbuffer[LR1110_CHIP_EUI_LENGTH] = { 0 };

  cbuffer[0] = ( uint8_t )( LR1110_GET_CHIP_EUI_OC >> 8 );
  cbuffer[1] = ( uint8_t )( LR1110_GET_CHIP_EUI_OC >> 0 );

  if (lr1110_spi_read( context, cbuffer, LR1110_CHIP_EUI_CMD_LENGTH, rbuffer, LR1110_CHIP_EUI_LENGTH ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

    HAL_DBG_TRACE_INFO("Chip EUI: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", rbuffer[0], rbuffer[1], rbuffer[2], rbuffer[3], rbuffer[4], rbuffer[5], rbuffer[6], rbuffer[7]);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to get LR1110 Chip EUI\r\n");
  }
}

void resetLR1110() {
  HAL_DBG_TRACE_INFO("Resetting LR1110... ");
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
  HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
}

void setupTCXO( const void* context ) {
  HAL_DBG_TRACE_INFO( "Setting up TCXO mode... " );

  uint8_t cbuffer[LR1110_SET_TCXO_MODE_CMD_LENGTH];

  cbuffer[0] = LR1110_GROUP_ID_SYSTEM;
  cbuffer[1] = LR1110_SET_TCXO_MODE_CMD;

  cbuffer[2] = ( uint8_t ) LR1110_TCXO_CTRL_1_8V;

  const uint32_t timeout = ( 5 * 1000 ) / 30.52;  // BOARD_TCXO_WAKEUP_TIME = 5

  cbuffer[3] = ( uint8_t )( timeout >> 16 );
  cbuffer[4] = ( uint8_t )( timeout >> 8 );
  cbuffer[5] = ( uint8_t )( timeout >> 0 );

  if (lr1110_spi_write( ((radio_t1*)context)->spi, cbuffer, LR1110_SET_TCXO_MODE_CMD_LENGTH, 1000 ) == LR1110_SPI_STATUS_OK) {
    HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);
  } else {
    HAL_DBG_TRACE_ERROR("Failed to set TCXO mode\r\n");
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */

  CRITICAL_SECTION_BEGIN( );

  HAL_DBG_TRACE_ERROR( "%s\n", __func__ );
  HAL_DBG_TRACE_ERROR( "PANIC" );

  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();

  /* Restart system */
  NVIC_SystemReset( );

  while (1) {  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
