/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdbool.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define RX_LED_Pin GPIO_PIN_0
#define RX_LED_GPIO_Port GPIOC
#define TX_LED_Pin GPIO_PIN_1
#define TX_LED_GPIO_Port GPIOC
#define RESET_Pin GPIO_PIN_0
#define RESET_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LNA_CTRL_Pin GPIO_PIN_0
#define LNA_CTRL_GPIO_Port GPIOB
#define NSS_Pin GPIO_PIN_8
#define NSS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define BUSY_Pin GPIO_PIN_3
#define BUSY_GPIO_Port GPIOB
#define EVENT_Pin GPIO_PIN_4
#define EVENT_GPIO_Port GPIOB
#define EVENT_EXTI_IRQn EXTI4_IRQn
#define SNIFFING_LED_Pin GPIO_PIN_5
#define SNIFFING_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

// UART COMMANDS
#define HAL_DBG_TRACE_COLOR_RED "\x1B[0;31m"
#define HAL_DBG_TRACE_COLOR_GREEN "\x1B[0;32m"
#define HAL_DBG_TRACE_COLOR_YELLOW "\x1B[0;33m"
#define HAL_DBG_TRACE_COLOR_DEFAULT "\x1B[0m"

#define HAL_DBG_TRACE_PRINTF( ... ) HAL_DBG_TRACE_PRINT( __VA_ARGS__ )

#define HAL_DBG_TRACE_MSG( msg )                             \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
        HAL_DBG_TRACE_PRINTF( msg );                         \
    } while( 0 );

#define HAL_DBG_TRACE_MSG_COLOR( msg, color )                \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( color );                       \
        HAL_DBG_TRACE_PRINTF( msg );                         \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_INFO( ... )                            \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_GREEN );   \
        HAL_DBG_TRACE_PRINTF( "INFO : " );                   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_INFO_VALUE( ... )                            \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_GREEN );   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_WARNING( ... )                         \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_YELLOW );  \
        HAL_DBG_TRACE_PRINTF( "WARN : " );                   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_ERROR( ... )                           \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_RED );     \
        HAL_DBG_TRACE_PRINTF( "ERROR : " );                   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

/*!
 * @brief Prints debug trace
 *
 * @param variadics arguments
 */
void HAL_DBG_TRACE_PRINT( const char* fmt, ... );

typedef struct configuration {
    GPIO_TypeDef* port;
    uint32_t      pin;
} gpio_t;

typedef struct hal_gpio_irq_s {
    gpio_t irq1;
    void*                context;
    void ( *callback )( void* context );
} hal_gpio_irq_t;

typedef struct {
    SPI_TypeDef*        spi;
    gpio_t              nss;
    gpio_t              reset;
    hal_gpio_irq_t      event;
    gpio_t              busy;
    SPI_HandleTypeDef * hspi;
} radio_t;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
