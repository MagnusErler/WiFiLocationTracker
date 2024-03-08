
#include "accelerometer.h"

#include "main.h"       // for HAL_DBG_TRACE-functions


/** I2C Device Address 8 bit format if SA0=0 -> 31 if SA0=1 -> 33 **/
#define LIS2DE12_I2C_ADD_H   0x33U

/** Device Identification (Who am I) **/
#define LIS2DE12_ID          0x33U

#define LIS2DE12_WHO_AM_I              0x0FU


#define SMTC_FAIL 0
#define SMTC_SUCCESS 1

I2C_HandleTypeDef _hi2c1;

/*!
 * @brief Internal device address size
 */
typedef enum
{
    I2C_ADDR_SIZE_8 = 0,
    I2C_ADDR_SIZE_16,
} i2c_addr_size;

static i2c_addr_size i2c_internal_addr_size;


static uint8_t i2c_read_buffer( uint8_t device_addr, uint16_t addr, uint8_t* buffer, uint16_t size )
{
    uint8_t  readStatus = SMTC_FAIL;
    uint16_t memAddSize = 0u;

    if( i2c_internal_addr_size == I2C_ADDR_SIZE_8 ) {
        memAddSize = I2C_MEMADD_SIZE_8BIT;
    } else {
        memAddSize = I2C_MEMADD_SIZE_16BIT;
    }
    if (HAL_I2C_Mem_Read(&_hi2c1, device_addr, addr, memAddSize, buffer, size, 2000) == HAL_OK) {
        readStatus = SMTC_SUCCESS;
    }

    return readStatus;
}

uint8_t hal_i2c_read_buffer( uint8_t device_addr, uint16_t addr, uint8_t* buffer, uint16_t size ) {
    return ( i2c_read_buffer( device_addr, addr, buffer, size ) );
}

int32_t lis2de12_read_reg( uint8_t reg, uint8_t* data, uint16_t len ) {
    return !hal_i2c_read_buffer( LIS2DE12_I2C_ADD_H, reg, data, len );
}

int32_t lis2de12_device_id_get( uint8_t* buff ) {
    return lis2de12_read_reg( LIS2DE12_WHO_AM_I, buff, 1 );
}

static uint8_t i2c_write_buffer( const uint32_t id, uint8_t device_addr, uint16_t addr, uint8_t* buffer, uint16_t size ) {
    uint8_t  write_status = SMTC_FAIL;
    uint16_t memAddSize   = 0u;

    if( i2c_internal_addr_size == I2C_ADDR_SIZE_8 ) {
        memAddSize = I2C_MEMADD_SIZE_8BIT;
    } else {
        memAddSize = I2C_MEMADD_SIZE_16BIT;
    }
    if (HAL_I2C_Mem_Write(&_hi2c1, device_addr, addr, memAddSize, buffer, size, 2000u) == HAL_OK) {
        write_status = SMTC_SUCCESS;
    }
    return write_status;
}

uint8_t hal_i2c_write_buffer( const uint32_t id, uint8_t device_addr, uint16_t addr, uint8_t* buffer, uint16_t size ) {
    if( i2c_write_buffer( id, device_addr, addr, buffer, size ) == SMTC_FAIL ) {
        // if first attempt fails due to an IRQ, try a second time
        if( i2c_write_buffer( id, device_addr, addr, buffer, size ) == SMTC_FAIL ) {
            return SMTC_FAIL;
        } else {
            return SMTC_SUCCESS;
        }
    } else {
        return SMTC_SUCCESS;
    }
}

int32_t lis2de12_write_reg( uint8_t reg, uint8_t* data, uint16_t len ) {
    int32_t ret;
    ret = hal_i2c_write_buffer( 1, LIS2DE12_I2C_ADD_H, reg, data, len );
    return !ret;
}

int32_t lis2de12_data_rate_set( lis2de12_odr_t val ) {
    HAL_DBG_TRACE_INFO("Setting data rate...");
    lis2de12_ctrl_reg1_t ctrl_reg1;

    int32_t ret;
    if( lis2de12_read_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 ) == 0 ) {
        ctrl_reg1.lpen = 1U;
        ctrl_reg1.odr  = ( uint8_t ) val;
        if( lis2de12_write_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        } else {
            HAL_DBG_TRACE_ERROR("Failed to set data rate\r\n");
            return -1;
        }
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set data rate\r\n");
        return -1;
    }
}

int32_t lis2de12_block_data_update_set( uint8_t val ) {
    HAL_DBG_TRACE_INFO("Setting block data update...\r\n");
    lis2de12_ctrl_reg4_t ctrl_reg4;

    int32_t ret = lis2de12_read_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 );
    if( ret == 0 ) {
        ctrl_reg4.bdu = !val;
        ret           = lis2de12_write_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 );
    }
    return ret;
}

int32_t lis2de12_fifo_mode_set( lis2de12_fm_t val ) {
    HAL_DBG_TRACE_INFO("Setting FIFO mode...\r\n");
    lis2de12_fifo_ctrl_reg_t fifo_ctrl_reg;

    int32_t ret = lis2de12_read_reg( LIS2DE12_FIFO_CTRL_REG, ( uint8_t* ) &fifo_ctrl_reg, 1 );
    if( ret == 0 ) {
        fifo_ctrl_reg.fm = ( uint8_t ) val;
        ret              = lis2de12_write_reg( LIS2DE12_FIFO_CTRL_REG, ( uint8_t* ) &fifo_ctrl_reg, 1 );
    }
    return ret;
}

int32_t lis2de12_full_scale_set( lis2de12_fs_t val ) {
    HAL_DBG_TRACE_INFO("Setting full scale...");
    lis2de12_ctrl_reg4_t ctrl_reg4;

    int32_t ret = lis2de12_read_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 );
    if( ret == 0 ) {
        ctrl_reg4.fs = ( uint8_t ) val;
        ret          = lis2de12_write_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 );
    }
    return ret;
}

int32_t lis2de12_high_pass_int_conf_set( lis2de12_hp_t val ) {
    HAL_DBG_TRACE_INFO("Setting high pass filter...");
    lis2de12_ctrl_reg2_t ctrl_reg2;

    int32_t ret = lis2de12_read_reg( LIS2DE12_CTRL_REG2, ( uint8_t* ) &ctrl_reg2, 1 );
    if( ret == 0 ) {
        ctrl_reg2.hp = ( uint8_t ) val;
        ret          = lis2de12_write_reg( LIS2DE12_CTRL_REG2, ( uint8_t* ) &ctrl_reg2, 1 );
    }
    return ret;
}

int32_t lis2de12_pin_int1_config_set( lis2de12_ctrl_reg3_t* val ) {
    HAL_DBG_TRACE_INFO("Setting pin int1 config...");
    if (lis2de12_write_reg( LIS2DE12_CTRL_REG3, ( uint8_t* ) val, 1 ) != 0) {
        HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
        return 0;
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set pin int1 config\r\n");
        return -1;
    }
    // int32_t ret;
    // ret = lis2de12_write_reg( LIS2DE12_CTRL_REG3, ( uint8_t* ) val, 1 );
    // return ret;
}

int32_t lis2de12_int1_pin_notification_mode_set( lis2de12_lir_int1_t val ) {
    HAL_DBG_TRACE_INFO("Setting int1 pin notification mode...");
    lis2de12_ctrl_reg5_t ctrl_reg5;
    int32_t              ret;

    ret = lis2de12_read_reg( LIS2DE12_CTRL_REG5, ( uint8_t* ) &ctrl_reg5, 1 );
    if( ret == 0 ) {
        ctrl_reg5.lir_int1 = ( uint8_t ) val;
        ret                = lis2de12_write_reg( LIS2DE12_CTRL_REG5, ( uint8_t* ) &ctrl_reg5, 1 );
    }
    return ret;
}

int32_t lis2de12_int1_gen_conf_set( lis2de12_int1_cfg_t* val ) {
    HAL_DBG_TRACE_INFO("Setting int1 gen conf...");
    if (lis2de12_write_reg( LIS2DE12_INT1_CFG, ( uint8_t* ) val, 1 ) != 0) {
        HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
        return 0;
    } else {
        HAL_DBG_TRACE_ERROR("Failed to set int1 gen conf\r\n");
        return -1;
    }
    // int32_t ret;
    // ret = lis2de12_write_reg( LIS2DE12_INT1_CFG, ( uint8_t* ) val, 1 );
    // return ret;
}

int32_t lis2de12_int1_gen_threshold_set( uint8_t val ) {
    HAL_DBG_TRACE_INFO("Setting int1 gen threshold...");
    lis2de12_int1_ths_t int1_ths;
    int32_t             ret;

    ret = lis2de12_read_reg( LIS2DE12_INT1_THS, ( uint8_t* ) &int1_ths, 1 );
    if( ret == 0 )
    {
        int1_ths.ths = val;
        ret          = lis2de12_write_reg( LIS2DE12_INT1_THS, ( uint8_t* ) &int1_ths, 1 );
    }
    return ret;
}

int32_t lis2de12_int1_gen_duration_set( uint8_t val ) {
    HAL_DBG_TRACE_INFO("Setting int1 gen duration...");
    lis2de12_int1_duration_t int1_duration;
    int32_t                  ret;

    ret = lis2de12_read_reg( LIS2DE12_INT1_DURATION, ( uint8_t* ) &int1_duration, 1 );
    if( ret == 0 )
    {
        int1_duration.d = val;
        ret             = lis2de12_write_reg( LIS2DE12_INT1_DURATION, ( uint8_t* ) &int1_duration, 1 );
    }
    return ret;
}

void init_accelerometer(I2C_HandleTypeDef hi2c1) {
    HAL_DBG_TRACE_INFO("Initilizing accelerometer...");

    _hi2c1 = hi2c1;

    int i = 0;
    uint8_t who_am_i;

    /* Check device ID */
    while( ( i <= 5 ) && ( who_am_i != LIS2DE12_ID ) ) {
        HAL_I2C_Mem_Read(&_hi2c1, LIS2DE12_I2C_ADD_H, LIS2DE12_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &who_am_i, 1, 5000);
        if( who_am_i != LIS2DE12_ID ) {
            if( i == 5 ) {
                HAL_DBG_TRACE_ERROR("LIS2DE12 not found\r\n");
                break;
            }
        }
        i++;
    }
    HAL_DBG_TRACE_INFO_VALUE("DONE (Device ID: 0x%02X)\r\n", who_am_i);

    /* Set Output Data Rate to 10Hz */
    lis2de12_data_rate_set( LIS2DE12_ODR_10Hz );

    /* Enable Block Data Update */
    lis2de12_block_data_update_set( PROPERTY_ENABLE );

    /* Enable bypass mode */
    lis2de12_fifo_mode_set( LIS2DE12_BYPASS_MODE );

    /* Set full scale to 2g */
    lis2de12_full_scale_set( LIS2DE12_2g );

    /* Motion detection setup */
    lis2de12_ctrl_reg1_t ctrl_reg1;
    lis2de12_read_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 );
    ctrl_reg1.xen  = 1;
    ctrl_reg1.yen  = 1;
    ctrl_reg1.zen  = 1;
    ctrl_reg1.lpen = 1;
    lis2de12_write_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 );

    lis2de12_high_pass_int_conf_set( LIS2DE12_ON_INT1_GEN );

    lis2de12_ctrl_reg3_t ctrl_reg3;
    ctrl_reg3.i1_zyxda    = 0;
    ctrl_reg3.i1_ia1      = 1;
    ctrl_reg3.i1_ia2      = 0;
    ctrl_reg3.i1_click    = 0;
    ctrl_reg3.i1_overrun  = 0;
    ctrl_reg3.i1_wtm      = 0;
    ctrl_reg3.not_used_01 = 0;
    ctrl_reg3.not_used_02 = 0;
    lis2de12_pin_int1_config_set( &ctrl_reg3 );

    lis2de12_int1_pin_notification_mode_set( LIS2DE12_INT1_LATCHED );

    lis2de12_int1_cfg_t  lis2de12_int1_cfg;
    lis2de12_int1_cfg.xhie = 1;
    lis2de12_int1_cfg.yhie = 1;
    lis2de12_int1_cfg.zhie = 1;
    lis2de12_int1_gen_conf_set( &lis2de12_int1_cfg );

    lis2de12_int1_gen_threshold_set( 4 );

    lis2de12_int1_gen_duration_set( 3 );
}