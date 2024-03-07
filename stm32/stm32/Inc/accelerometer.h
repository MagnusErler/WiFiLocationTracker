#include <stdint.h>

#include "stm32l4xx_ll_i2c.h"


typedef enum {
  LIS2DE12_POWER_DOWN                      = 0x00,
  LIS2DE12_ODR_1Hz                         = 0x01,
  LIS2DE12_ODR_10Hz                        = 0x02,
  LIS2DE12_ODR_25Hz                        = 0x03,
  LIS2DE12_ODR_50Hz                        = 0x04,
  LIS2DE12_ODR_100Hz                       = 0x05,
  LIS2DE12_ODR_200Hz                       = 0x06,
  LIS2DE12_ODR_400Hz                       = 0x07,
  LIS2DE12_ODR_1kHz620_LP                  = 0x08,
  LIS2DE12_ODR_5kHz376_LP_1kHz344_NM_HP    = 0x09,
} lis2de12_odr_t;

typedef struct {
  uint8_t xen               : 1;
  uint8_t yen               : 1;
  uint8_t zen               : 1;
  uint8_t lpen              : 1;
  uint8_t odr               : 4;
} lis2de12_ctrl_reg1_t;

#define LIS2DE12_CTRL_REG1             0x20U


void init_accelerometer(I2C_HandleTypeDef hi2c1);