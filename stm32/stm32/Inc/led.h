#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

/*!
 * @brief Turn on LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 */
void turnOnLED(GPIO_TypeDef* LED_GPIO_Port, const uint16_t LED_Pin);

/*!
 * @brief Turn off LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 */
void turnOffLED(GPIO_TypeDef* LED_GPIO_Port, const uint16_t LED_Pin);

/*!
 * @brief Toggle LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 */
void toggleLED(GPIO_TypeDef* LED_GPIO_Port, const uint16_t LED_Pin);

/*!
 * @brief Blink LED
 *
 * @param [in] LED_GPIO_Port
 * @param [in] LED_Pin
 * @param [in] period
 * @param [in] count
 * @param [in] start
 */
void blinkLED(GPIO_TypeDef* LED_GPIO_Port, const uint16_t LED_Pin, const uint32_t period, const uint8_t count);