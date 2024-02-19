#include "led.h"

#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

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
  //EXAMPLE: blinkLED(GPIOC, RX_LED_Pin|TX_LED_Pin, 100, 5, true);
  if (start) {
    for (uint8_t i = 0; i < count; i++) {
      turnOnLED(LED_GPIO_Port, LED_Pin);
      HAL_Delay(period);
      turnOffLED(LED_GPIO_Port, LED_Pin);
      HAL_Delay(period);
    }
  }
}