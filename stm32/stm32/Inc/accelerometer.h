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

#define LIS2DE12_CTRL_REG4             0x23U
typedef struct {
  uint8_t sim               : 1;
  uint8_t st                : 2;
  uint8_t not_used_01       : 1;
  uint8_t fs                : 2;
  uint8_t not_used_02       : 1;
  uint8_t bdu               : 1;
} lis2de12_ctrl_reg4_t;

typedef enum {
  LIS2DE12_BYPASS_MODE           = 0,
  LIS2DE12_FIFO_MODE             = 1,
  LIS2DE12_DYNAMIC_STREAM_MODE   = 2,
  LIS2DE12_STREAM_TO_FIFO_MODE   = 3,
} lis2de12_fm_t;

#define LIS2DE12_FIFO_READ_START      0x28U
#define LIS2DE12_OUT_X_H              0x29U
#define LIS2DE12_OUT_Y_H              0x2BU
#define LIS2DE12_OUT_Z_H              0x2DU
#define LIS2DE12_FIFO_CTRL_REG        0x2EU
typedef struct {
  uint8_t fth               : 5;
  uint8_t tr                : 1;
  uint8_t fm                : 2;
} lis2de12_fifo_ctrl_reg_t;

typedef enum {
  LIS2DE12_2g   = 0,
  LIS2DE12_4g   = 1,
  LIS2DE12_8g   = 2,
  LIS2DE12_16g  = 3,
} lis2de12_fs_t;

typedef enum {
  LIS2DE12_DISC_FROM_INT_GENERATOR  = 0,
  LIS2DE12_ON_INT1_GEN              = 1,
  LIS2DE12_ON_INT2_GEN              = 2,
  LIS2DE12_ON_TAP_GEN               = 4,
  LIS2DE12_ON_INT1_INT2_GEN         = 3,
  LIS2DE12_ON_INT1_TAP_GEN          = 5,
  LIS2DE12_ON_INT2_TAP_GEN          = 6,
  LIS2DE12_ON_INT1_INT2_TAP_GEN     = 7,
} lis2de12_hp_t;

#define LIS2DE12_CTRL_REG2             0x21U
typedef struct {
  uint8_t hp                : 3; /* HPCLICK + HP_IA2 + HP_IA1 -> HP */
  uint8_t fds               : 1;
  uint8_t hpcf              : 2;
  uint8_t hpm               : 2;
} lis2de12_ctrl_reg2_t;

#define LIS2DE12_CTRL_REG3             0x22U
typedef struct {
  uint8_t not_used_01       : 1;
  uint8_t i1_overrun        : 1;
  uint8_t i1_wtm            : 1;
  uint8_t not_used_02       : 1;
  uint8_t i1_zyxda          : 1;
  uint8_t i1_ia2            : 1;
  uint8_t i1_ia1            : 1;
  uint8_t i1_click          : 1;
} lis2de12_ctrl_reg3_t;

typedef enum {
  LIS2DE12_INT1_PULSED   = 0,
  LIS2DE12_INT1_LATCHED  = 1,
} lis2de12_lir_int1_t;

#define LIS2DE12_INT1_CFG             0x30U
typedef struct {
  uint8_t xlie              : 1;
  uint8_t xhie              : 1;
  uint8_t ylie              : 1;
  uint8_t yhie              : 1;
  uint8_t zlie              : 1;
  uint8_t zhie              : 1;
  uint8_t _6d               : 1;
  uint8_t aoi               : 1;
} lis2de12_int1_cfg_t;

#define LIS2DE12_CTRL_REG5             0x24U
typedef struct {
  uint8_t d4d_int2          : 1;
  uint8_t lir_int2          : 1;
  uint8_t d4d_int1          : 1;
  uint8_t lir_int1          : 1;
  uint8_t not_used_01       : 2;
  uint8_t fifo_en           : 1;
  uint8_t boot              : 1;
} lis2de12_ctrl_reg5_t;

#define LIS2DE12_INT1_THS             0x32U
typedef struct {
  uint8_t ths               : 7;
  uint8_t not_used_01       : 1;
} lis2de12_int1_ths_t;

#define LIS2DE12_INT1_DURATION        0x33U
typedef struct {
  uint8_t d                 : 7;
  uint8_t not_used_01       : 1;
} lis2de12_int1_duration_t;

#define LIS2DE12_STATUS_REG_AUX        0x07U
typedef struct {
  uint8_t not_used_01       : 2;
  uint8_t tda               : 1;
  uint8_t not_used_02       : 3;
  uint8_t tor               : 1;
  uint8_t not_used_03       : 1;
} lis2de12_status_reg_aux_t;

#define LIS2DE12_CTRL_REG1             0x20U

#define PROPERTY_ENABLE                 (1U)

#define LIS2DE12_OUT_TEMP_L            0x0CU
#define LIS2DE12_OUT_TEMP_H            0x0DU
#define LIS2DE12_WHO_AM_I              0x0FU

typedef enum {
  LIS2DE12_TEMP_DISABLE  = 0,
  LIS2DE12_TEMP_ENABLE   = 3,
} lis2de12_temp_en_t;

#define LIS2DE12_TEMP_CFG_REG          0x1FU
typedef struct {
  uint8_t not_used_01       : 6;
  uint8_t temp_en           : 2;
} lis2de12_temp_cfg_reg_t;

void initLIS2DE12(I2C_HandleTypeDef hi2c1);

int16_t acc_get_temperature( void );

int32_t enableLIS2DE12_Temperature_Sensor( void );