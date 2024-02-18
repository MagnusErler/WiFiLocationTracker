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

#include <stdlib.h>  // used for malloc function
#include <string.h>  // used for strlen function
#include <stdarg.h> // used for va_list, va_start, va_end functions
#include <stdio.h>  // used for vsprintf function


#include "spi.h"

#include "rtc.h"


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
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */





















/*!
 * @brief Type to store a Wi-Fi datarate info byte
 */
typedef uint8_t lr11xx_wifi_datarate_info_byte_t1;

/*!
 * @brief Type to store a Wi-Fi channel info byte
 */
typedef uint8_t lr11xx_wifi_channel_info_byte_t1;

/*!
 * @brief Type to store a Wi-Fi frame type info byte
 */
typedef uint8_t lr11xx_wifi_frame_type_info_byte_t1;

/*!
 * @brief Type to store a MAC address
 */
typedef uint8_t lr11xx_wifi_mac_address_t1[6];

typedef struct lr11xx_wifi_basic_complete_result_s1
{
    lr11xx_wifi_datarate_info_byte_t1   data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1    channel_info_byte;
    int8_t                             rssi;
    lr11xx_wifi_frame_type_info_byte_t1 frame_type_info_byte;
    lr11xx_wifi_mac_address_t1          mac_address;
    int16_t                            phi_offset;
    uint64_t timestamp_us;  //!< Indicate the up-time of the Access Point transmitting the Beacon [us]
    uint16_t beacon_period_tu;
} lr11xx_wifi_basic_complete_result_t1;

typedef struct lr11xx_wifi_basic_mac_type_channel_result_s1
{
    lr11xx_wifi_datarate_info_byte_t1 data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1  channel_info_byte;
    int8_t                           rssi;
    lr11xx_wifi_mac_address_t1        mac_address;
} lr11xx_wifi_basic_mac_type_channel_result_t1;

typedef enum
{
    LR11XX_WIFI_NO_CHANNEL   = 0x00,
    LR11XX_WIFI_CHANNEL_1    = 0x01,  //!< Channel at frequency 2.412 GHz
    LR11XX_WIFI_CHANNEL_2    = 0x02,  //!< Channel at frequency 2.417 GHz
    LR11XX_WIFI_CHANNEL_3    = 0x03,  //!< Channel at frequency 2.422 GHz
    LR11XX_WIFI_CHANNEL_4    = 0x04,  //!< Channel at frequency 2.427 GHz
    LR11XX_WIFI_CHANNEL_5    = 0x05,  //!< Channel at frequency 2.432 GHz
    LR11XX_WIFI_CHANNEL_6    = 0x06,  //!< Channel at frequency 2.437 GHz
    LR11XX_WIFI_CHANNEL_7    = 0x07,  //!< Channel at frequency 2.442 GHz
    LR11XX_WIFI_CHANNEL_8    = 0x08,  //!< Channel at frequency 2.447 GHz
    LR11XX_WIFI_CHANNEL_9    = 0x09,  //!< Channel at frequency 2.452 GHz
    LR11XX_WIFI_CHANNEL_10   = 0x0A,  //!< Channel at frequency 2.457 GHz
    LR11XX_WIFI_CHANNEL_11   = 0x0B,  //!< Channel at frequency 2.462 GHz
    LR11XX_WIFI_CHANNEL_12   = 0x0C,  //!< Channel at frequency 2.467 GHz
    LR11XX_WIFI_CHANNEL_13   = 0x0D,  //!< Channel at frequency 2.472 GHz
    LR11XX_WIFI_CHANNEL_14   = 0x0E,  //!< Channel at frequency 2.484 GHz
    LR11XX_WIFI_ALL_CHANNELS = 0x0F,
} lr11xx_wifi_channel_t1;

typedef uint8_t lr11xx_wifi_country_code_str_t1[2];

typedef struct lr11xx_wifi_fcs_info_byte_s1
{
    bool is_fcs_ok;       //!< True if the LR11XX has checked the FCS and the check succeeded
    bool is_fcs_checked;  //!< True if the LR11XX has checked the FCS
} lr11xx_wifi_fcs_info_byte_t1;

typedef struct
{
    lr11xx_wifi_datarate_info_byte_t1 data_rate_info_byte;
    lr11xx_wifi_channel_info_byte_t1  channel_info_byte;
    int8_t                           rssi;
    uint8_t                          rate;           //!< Rate index
    uint16_t                         service;        //!< Service value
    uint16_t                         length;         //!< Length of MPDU (in microseconds for WiFi B, bytes for WiFi G)
    uint16_t                         frame_control;  //!< Frame Control structure
    lr11xx_wifi_mac_address_t1        mac_address_1;
    lr11xx_wifi_mac_address_t1        mac_address_2;
    lr11xx_wifi_mac_address_t1        mac_address_3;
    uint64_t                         timestamp_us;  //!< Indicate the up-time of the Access Point
                                                    //!< transmitting the Beacon [us]
    uint16_t beacon_period_tu;
    uint16_t seq_control;                                 //!< Sequence Control value
    uint8_t  ssid_bytes[32];  //!< Service Set
                                                          //!< IDentifier
    lr11xx_wifi_channel_t1          current_channel;       //!< Current channel indicated in the Wi-Fi frame
    lr11xx_wifi_country_code_str_t1 country_code;          //!< Country Code
    uint8_t                        io_regulation;         //!< Input Output Regulation
    lr11xx_wifi_fcs_info_byte_t1    fcs_check_byte;        //!< Frame Check Sequence info
    int16_t                        phi_offset;
} lr11xx_wifi_extended_full_result_t1;

typedef union
{
    lr11xx_wifi_basic_complete_result_t1*         basic_complete;
    lr11xx_wifi_basic_mac_type_channel_result_t1* basic_mac_type_channel;
    lr11xx_wifi_extended_full_result_t1*          extended_complete;
} lr11xx_wifi_result_interface_t1;

typedef enum
{
    LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE,          //!< Basic complete result format: @ref
                                                       //!< lr11xx_wifi_basic_complete_result_t
    LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL,  //!<  Basic MAC/type/channel result format: @ref
                                                       //!<  lr11xx_wifi_basic_mac_type_channel_result_t
    LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL,  //!< Extended full result format: @ref lr11xx_wifi_extended_full_result_t
} lr11xx_wifi_result_format_t1;

typedef enum lr11xx_hal_status_e1
{
    LR11XX_HAL_STATUS_OK1    = 0,
    LR11XX_HAL_STATUS_ERROR1 = 3,
} lr11xx_hal_status_t1;

typedef enum
{
    LR11XX_WIFI_TYPE_SCAN_B     = 0x01,  //!< Wi-Fi B
    LR11XX_WIFI_TYPE_SCAN_G     = 0x02,  //!< Wi-Fi G
    LR11XX_WIFI_TYPE_SCAN_N     = 0x03,  //!< Wi-Fi N
    LR11XX_WIFI_TYPE_SCAN_B_G_N = 0x04,  //!< Wi-Fi B and Wi-Fi G/N
} lr11xx_wifi_signal_type_scan_t1;

typedef uint16_t lr11xx_wifi_channel_mask_t1;

typedef enum
{
    LR11XX_WIFI_SCAN_MODE_BEACON =
        1,  //!< Exposes Beacons and Probe Responses Access Points frames until Period Beacon field (Basic result)
    LR11XX_WIFI_SCAN_MODE_BEACON_AND_PKT =
        2,  //!< Exposes some Management Access Points frames until Period Beacon field, and some other packets frame
            //!< until third Mac Address field (Basic result)
    LR11XX_WIFI_SCAN_MODE_FULL_BEACON =
        4,  //!< Exposes Beacons and Probes Responses Access Points frames until Frame Check Sequence (FCS) field
            //!< (Extended result). In this mode, only signal type LR11XX_WIFI_TYPE_SCAN_B is executed and other signal
            //!< types are silently discarded.
    LR11XX_WIFI_SCAN_MODE_UNTIL_SSID = 5,  //!< Exposes Beacons and Probes Responses Access Points frames until the end
                                           //!< of SSID field (Extended result) - available since firmware 0x0306
} lr11xx_wifi_mode_t1;

typedef enum lr11xx_status_e1
{
    LR11XX_STATUS_OK1    = 0,
    LR11XX_STATUS_ERROR1 = 3,
} lr11xx_status_t1;





















/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
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

// /*!
//  * @brief Get LR1110 GNSS version
//  *
//  * @param [in] context Radio abstraction
//  */
// static void getLR1110_GNSS_Version( const void* context);

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
 * @brief Scan Wi-Fi networks
 *
 * @param [in] context Radio abstraction
 */
static void scanWiFiNetworks( const void* context);

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

static void scanWiFiNetworks( const void* context );

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

  int a = 0;

  void* lr1110_context = (void*) malloc(sizeof(radio_t));
  ((radio_t*)lr1110_context)->spi             = SPI1;
  ((radio_t*)lr1110_context)->nss.port        = GPIOA;
  ((radio_t*)lr1110_context)->nss.pin         = NSS_Pin;
  ((radio_t*)lr1110_context)->reset.port      = GPIOA;
  ((radio_t*)lr1110_context)->reset.pin       = RESET_Pin;
  // ((radio_t*)lr1110_context)->event.irq1.port = GPIOB;
  // ((radio_t*)lr1110_context)->event.irq1.pin = EVENT_Pin;
  //((radio_t*)lr1110_context)->event.pin       = EVENT_Pin;
  //((radio_t*)lr1110_context)->event.callback  = radio_event_callback;
  ((radio_t*)lr1110_context)->event.context   = ( ( radio_t* ) lr1110_context );
  ((radio_t*)lr1110_context)->busy.port       = GPIOB;
  ((radio_t*)lr1110_context)->busy.pin        = BUSY_Pin;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */


  HAL_DBG_TRACE_MSG("-----------------------------\r\n\r\n");
  resetLR1110();

  blinkLED(GPIOC, RX_LED_Pin|TX_LED_Pin, 100, 5, true);

  setupTCXO(lr1110_context);

  getLR1110_Bootloader_Version(lr1110_context);   // First call to this function is always returning 0.0 no matter if calling for bootloader og wifi version
  getLR1110_Bootloader_Version(lr1110_context);
  getLR1110_WiFi_Version(lr1110_context);
  getLR1110_Chip_EUI(lr1110_context);


  //smtc_modem_init( &modem_radio, &get_event );

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    
    //turnOnLED(GPIOC, RX_LED_Pin);
    //turnOffLED(GPIOC, RX_LED_Pin);
    //toggleLED(GPIOC, RX_LED_Pin|TX_LED_Pin);

    HAL_DBG_TRACE_PRINTF("2a = %d\r\n", a++);

    getLR1110_Temperature(lr1110_context);

    //scanWiFiNetworks(lr1110_context);

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

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

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

void hal_mcu_trace_print( const char* fmt, ... ) {
  va_list argp;
  va_start( argp, fmt );

  char string[255];
  if( 0 < vsprintf( string, fmt, argp ) ) {
      HAL_UART_Transmit(&huart2, (uint8_t*) string, (uint16_t) strlen((const char*) string), 0xffffff);
  }

  va_end( argp );
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

// void getLR1110_GNSS_Version( const void* context ) {
//   HAL_DBG_TRACE_INFO("Getting GNSS version... ");

//   uint8_t cbuffer[LR1110_GNSS_VERSION_CMD_LENGTH];
//   uint8_t rbuffer[LR1110_GNSS_VERSION_LENGTH] = { 0 };

//   cbuffer[0] = LR1110_GROUP_ID_GNSS;
//   cbuffer[1] = LR1110_GNSS_READ_FW_VERSION_CMD;

//   if (lr1110_spi_read( context, cbuffer, LR1110_GNSS_VERSION_CMD_LENGTH, rbuffer, LR1110_GNSS_VERSION_LENGTH ) == LR1110_SPI_STATUS_OK) {
//     HAL_DBG_TRACE_MSG_COLOR("DONE\r\n", HAL_DBG_TRACE_COLOR_GREEN);

//     HAL_DBG_TRACE_INFO("GNSS firmware version = %d\n\r", rbuffer[0]);
//     HAL_DBG_TRACE_INFO("GNSS almanac version = %d\n\r", rbuffer[1]);
//   } else {
//     HAL_DBG_TRACE_ERROR("Failed to get GNSS version\r\n");
//   }
// }

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













lr11xx_status_t1 lr11xx_wifi_scan1( const void* context, const lr11xx_wifi_signal_type_scan_t1 signal_type,
                                  const lr11xx_wifi_channel_mask_t1 channels, const lr11xx_wifi_mode_t1 scan_mode,
                                  const uint8_t max_results, const uint8_t nb_scan_per_channel,
                                  const uint16_t timeout_in_ms, const bool abort_on_timeout ) {
    const uint8_t cbuffer[2+9] = {
        ( uint8_t ) ( 0x0300 >> 8 ),
        ( uint8_t ) ( 0x0300 >> 0 ),
        ( uint8_t ) signal_type,
        ( uint8_t ) ( channels >> 8 ),
        ( uint8_t ) ( channels >> 0 ),
        ( uint8_t ) scan_mode,
        max_results,
        nb_scan_per_channel,
        ( uint8_t ) ( timeout_in_ms >> 8 ),
        ( uint8_t ) ( timeout_in_ms >> 0 ),
        ( uint8_t ) ( ( abort_on_timeout == true ) ? 1 : 0 ),
    };

    if ( lr1110_spi_write( ((radio_t*)context)->spi, cbuffer, 2+9, 1000 ) == LR1110_SPI_STATUS_OK ) {
        return LR11XX_STATUS_OK1;
    } else {
        return LR11XX_STATUS_ERROR1;
    }
}

static uint8_t lr11xx_wifi_get_result_size_from_format1( const lr11xx_wifi_result_format_t1 format ) {
    uint8_t result_size = 0;
    switch( format )
    {
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE:
    {
        result_size = 22;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL:
    {
        result_size = 9;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL:
    {
        result_size = 79;
        break;
    }
    }
    return result_size;
}

static uint8_t lr11xx_wifi_get_format_code1( const lr11xx_wifi_result_format_t1 format )
{
    uint8_t format_code = 0x00;
    switch( format )
    {
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE:
    {
        format_code = 0x01;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL:
    {
        format_code = 0x04;
        break;
    }
    case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL:
    {
        format_code = 0x01;
        break;
    }
    }
    return format_code;
}

static lr11xx_hal_status_t1 lr11xx_wifi_read_results_helper1( const void* context, const uint8_t start_index,
                                                            const uint8_t n_elem, uint8_t* buffer,
                                                            const lr11xx_wifi_result_format_t1 result_format )
{
    const uint8_t  size_single_elem   = lr11xx_wifi_get_result_size_from_format1( result_format );
    const uint8_t  result_format_code = lr11xx_wifi_get_format_code1( result_format );
    const uint8_t  cbuffer[2+3] = { ( uint8_t ) ( 0x0306 >> 8 ),
                                                                  ( uint8_t ) ( 0x0306 & 0x00FF ),
                                                                  start_index, n_elem, result_format_code };
    const uint16_t size_total                                  = n_elem * size_single_elem;
    //return (lr11xx_hal_status_t1) lr11xx_hal_read( context, cbuffer, 2+3, buffer, size_total );
    return (lr11xx_hal_status_t1) lr1110_spi_read( ((radio_t*)context)->spi, cbuffer, 2+3, buffer, size_total );
}

static void lr11xx_wifi_read_mac_address_from_buffer1( const uint8_t* buffer, const uint16_t index_in_buffer,
                                                      lr11xx_wifi_mac_address_t1 mac_address )
{
    for( uint8_t field_mac_index = 0; field_mac_index < 6; field_mac_index++ )
    {
        mac_address[field_mac_index] = buffer[index_in_buffer + field_mac_index];
    }
}

static uint16_t uint16_from_array1( const uint8_t* array, const uint16_t index )
{
    return ( uint16_t ) ( array[index] << 8 ) + ( ( uint16_t ) ( array[index + 1] ) );
}

static uint64_t uint64_from_array1( const uint8_t* array, const uint16_t index )
{
    return ( ( uint64_t ) ( array[index] ) << 56 ) + ( ( uint64_t ) ( array[index + 1] ) << 48 ) +
           ( ( uint64_t ) ( array[index + 2] ) << 40 ) + ( ( uint64_t ) ( array[index + 3] ) << 32 ) +
           ( ( uint64_t ) ( array[index + 4] ) << 24 ) + ( ( uint64_t ) ( array[index + 5] ) << 16 ) +
           ( ( uint64_t ) ( array[index + 6] ) << 8 ) + ( uint64_t ) ( array[index + 7] );
}

static void interpret_basic_complete_result_from_buffer1( const uint8_t  nb_results,
                                                         const uint8_t  index_result_start_writing,
                                                         const uint8_t* buffer,
                                                         lr11xx_wifi_basic_complete_result_t1* result )
{
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ )
    {
        const uint16_t                       local_index_start = 22 * result_index;
        lr11xx_wifi_basic_complete_result_t1* local_wifi_result = &result[index_result_start_writing + result_index];
        local_wifi_result->data_rate_info_byte                 = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte                   = buffer[local_index_start + 1];
        local_wifi_result->rssi                                = buffer[local_index_start + 2];
        local_wifi_result->frame_type_info_byte                = buffer[local_index_start + 3];
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 4, local_wifi_result->mac_address );
        local_wifi_result->phi_offset       = uint16_from_array1( buffer, local_index_start + 10 );
        local_wifi_result->timestamp_us     = uint64_from_array1( buffer, local_index_start + 12 );
        local_wifi_result->beacon_period_tu = uint16_from_array1( buffer, local_index_start + 20 );
    }
}

static void interpret_basic_mac_type_channel_result_from_buffer1( const uint8_t  nb_results,
                                                                 const uint8_t  index_result_start_writing,
                                                                 const uint8_t* buffer,
                                                                 lr11xx_wifi_basic_mac_type_channel_result_t1* result )
{
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ )
    {
        const uint16_t local_index_start = 9 * result_index;
        lr11xx_wifi_basic_mac_type_channel_result_t1* local_wifi_result =
            &result[index_result_start_writing + result_index];
        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 3, local_wifi_result->mac_address );
    }
}

void interpret_extended_full_result_from_buffer1( const uint8_t nb_results, const uint8_t index_result_start_writing,
                                                 const uint8_t* buffer, lr11xx_wifi_extended_full_result_t1* result )
{
    for( uint8_t result_index = 0; result_index < nb_results; result_index++ )
    {
        const uint16_t local_index_start = 79 * result_index;
        lr11xx_wifi_extended_full_result_t1* local_wifi_result = &result[index_result_start_writing + result_index];

        local_wifi_result->data_rate_info_byte = buffer[local_index_start + 0];
        local_wifi_result->channel_info_byte   = buffer[local_index_start + 1];
        local_wifi_result->rssi                = buffer[local_index_start + 2];
        local_wifi_result->rate                = buffer[local_index_start + 3];
        local_wifi_result->service             = uint16_from_array1( buffer, local_index_start + 4 );
        local_wifi_result->length              = uint16_from_array1( buffer, local_index_start + 6 );
        local_wifi_result->frame_control       = uint16_from_array1( buffer, local_index_start + 8 );
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 10, local_wifi_result->mac_address_1 );
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 16, local_wifi_result->mac_address_2 );
        lr11xx_wifi_read_mac_address_from_buffer1( buffer, local_index_start + 22, local_wifi_result->mac_address_3 );
        local_wifi_result->timestamp_us     = uint64_from_array1( buffer, local_index_start + 28 );
        local_wifi_result->beacon_period_tu = uint16_from_array1( buffer, local_index_start + 36 );
        local_wifi_result->seq_control      = uint16_from_array1( buffer, local_index_start + 38 );
        for( uint8_t ssid_index = 0; ssid_index < 32; ssid_index++ )
        {
            local_wifi_result->ssid_bytes[ssid_index] = buffer[local_index_start + ssid_index + 40];
        }
        local_wifi_result->current_channel               = buffer[local_index_start + 72];
        local_wifi_result->country_code[0]               = buffer[local_index_start + 73];
        local_wifi_result->country_code[1]               = buffer[local_index_start + 74];
        local_wifi_result->io_regulation                 = buffer[local_index_start + 75];
        local_wifi_result->fcs_check_byte.is_fcs_checked = ( ( buffer[local_index_start + 76] & 0x01 ) == 0x01 );
        local_wifi_result->fcs_check_byte.is_fcs_ok      = ( ( buffer[local_index_start + 76] & 0x02 ) == 0x02 );
        local_wifi_result->phi_offset                    = uint16_from_array1( buffer, local_index_start + 77 );
    }
}

static void generic_results_interpreter1( const uint8_t n_result_to_parse, const uint8_t index_result_start_writing,
                                         const uint8_t* buffer, lr11xx_wifi_result_interface_t1 result_interface,
                                         const lr11xx_wifi_result_format_t1 format_code )
{
    switch( format_code )
    {
    case LR11XX_WIFI_RESULT_FORMAT_BASIC_COMPLETE:
    {
        interpret_basic_complete_result_from_buffer1( n_result_to_parse, index_result_start_writing, buffer,
                                                     result_interface.basic_complete );
        break;
    }

    case LR11XX_WIFI_RESULT_FORMAT_BASIC_MAC_TYPE_CHANNEL:
    {
        interpret_basic_mac_type_channel_result_from_buffer1( n_result_to_parse, index_result_start_writing, buffer,
                                                             result_interface.basic_mac_type_channel );
        break;
    }

    case LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL:
    {
        interpret_extended_full_result_from_buffer1( n_result_to_parse, index_result_start_writing, buffer,
                                                    result_interface.extended_complete );
        break;
    }
    }
}

uint8_t min( const uint8_t a, const uint8_t b )
{
    return ( a < b ) ? a : b;
}

static lr11xx_status_t1 fetch_and_aggregate_all_results1( const void* context, const uint8_t index_result_start,
                                                        const uint8_t                     nb_results,
                                                        const uint8_t                     nb_results_per_chunk_max,
                                                        const lr11xx_wifi_result_format_t1 result_format_code,
                                                        uint8_t*                          result_buffer,
                                                        lr11xx_wifi_result_interface_t1    result_structures )
{
    uint8_t index_to_read              = index_result_start;
    uint8_t index_result_start_writing = 0;
    uint8_t remaining_results          = nb_results;

    lr11xx_hal_status_t1 hal_status = LR11XX_HAL_STATUS_OK1;
    while( remaining_results > 0 )
    {
        uint8_t results_to_read = min( remaining_results, nb_results_per_chunk_max );

        lr11xx_hal_status_t1 local_hal_status = lr11xx_wifi_read_results_helper1( context, index_to_read, results_to_read,
                                                                                result_buffer, result_format_code );
        if( local_hal_status != LR11XX_HAL_STATUS_OK1 )
        {
            return ( lr11xx_status_t1 ) local_hal_status;
        }

        generic_results_interpreter1( results_to_read, index_result_start_writing, result_buffer, result_structures,
                                     result_format_code );

        // Reset the content of the result_buffer in case there are still results to fetch
        {
            const uint16_t result_buffer_size = lr11xx_wifi_get_result_size_from_format1( result_format_code ) * ( min( ( 1020 ) / ( lr11xx_wifi_get_result_size_from_format1( result_format_code ) ), 32 ) );
            for( uint16_t index = 0; index < result_buffer_size; index++ )
            {
                result_buffer[index] = 0;
            }
        }

        index_to_read += results_to_read;
        index_result_start_writing += results_to_read;
        remaining_results -= results_to_read;
    }
    return ( lr11xx_status_t1 ) hal_status;
}

lr11xx_status_t1 lr11xx_wifi_read_extended_full_results1( const void* context, const uint8_t start_result_index,
                                                        const uint8_t                       nb_results,
                                                        lr11xx_wifi_extended_full_result_t1* results )
{
    uint8_t result_buffer1[1020] = { 0 };      // uint8_t result_buffer1[79 * ( MIN( ( 1020 ) / ( 79 ), 32 ) )] = { 0 };
    const uint8_t nb_results_per_chunk_max = 12;  // const uint8_t nb_results_per_chunk_max = MIN( ( 1020 ) / ( 79 ), 32 );

    lr11xx_wifi_result_interface_t1 result_interface = { 0 };
    result_interface.extended_complete              = results;

    return fetch_and_aggregate_all_results1( context, start_result_index, nb_results, nb_results_per_chunk_max,
                                            LR11XX_WIFI_RESULT_FORMAT_EXTENDED_FULL, result_buffer1, result_interface );
}

static void scanWiFiNetworks( const void* context ) {
    HAL_DBG_TRACE_PRINTF( "\r\n-----------------WIFI Scan-----------------\r\n" );

    if (lr11xx_wifi_scan1( context, LR11XX_WIFI_TYPE_SCAN_B_G_N, 0x3FFF, LR11XX_WIFI_SCAN_MODE_UNTIL_SSID, 12, 3, 110, true ) != LR11XX_STATUS_OK1) {
        HAL_DBG_TRACE_ERROR( "Failed to scan Wi-Fi networks\r\n" );
        return;
    }
    
    HAL_Delay( 110 );

    lr11xx_wifi_extended_full_result_t1 wifi_scan_result;
    for( int i = 0; i < 2; i++ ) {
        if (lr11xx_wifi_read_extended_full_results1( context, i, 1, &wifi_scan_result ) != LR11XX_STATUS_OK1) {
            HAL_DBG_TRACE_ERROR( "Failed to read Wi-Fi networks\r\n" );
            return;
        }
        HAL_DBG_TRACE_PRINTF( "wifi %d: SSID: %s, ", i, wifi_scan_result.ssid_bytes );
        HAL_DBG_TRACE_PRINTF( "mac: %02x:%02x:%02x:%02x:%02x:%02x, rssi: %d\r\n", wifi_scan_result.mac_address_3[0], wifi_scan_result.mac_address_3[1], wifi_scan_result.mac_address_3[2], wifi_scan_result.mac_address_3[3], wifi_scan_result.mac_address_3[4], wifi_scan_result.mac_address_3[5], wifi_scan_result.rssi );
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

  if (lr1110_spi_write( ((radio_t*)context)->spi, cbuffer, LR1110_SET_TCXO_MODE_CMD_LENGTH, 1000 ) == LR1110_SPI_STATUS_OK) {
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
