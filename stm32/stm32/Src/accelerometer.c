
#include "accelerometer.h"

#include "main.h"       // for HAL_DBG_TRACE-functions


/** I2C Device Address 8 bit format if SA0=0 -> 31 if SA0=1 -> 33 **/
#define LIS2DE12_I2C_ADD_H   0x33U




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


// static uint8_t i2c_read_buffer( uint8_t device_addr, uint16_t addr, uint8_t* buffer, uint16_t size )
// {
//     uint8_t  readStatus = SMTC_FAIL;
//     uint16_t memAddSize = 0u;

//     if( i2c_internal_addr_size == I2C_ADDR_SIZE_8 ) {
//         memAddSize = I2C_MEMADD_SIZE_8BIT;
//     } else {
//         memAddSize = I2C_MEMADD_SIZE_16BIT;
//     }
//     if (HAL_I2C_Mem_Read(&_hi2c1, device_addr, addr, memAddSize, buffer, size, 2000) == HAL_OK) {
//         readStatus = SMTC_SUCCESS;
//     }

//     return readStatus;
// }

uint8_t hal_i2c_read_buffer( uint8_t device_addr, uint16_t addr, uint8_t* buffer, uint16_t size ) {
    //return ( i2c_read_buffer( device_addr, addr, buffer, size ) );

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

int32_t lis2de12_read_reg( uint8_t reg, uint8_t* data, uint16_t len ) {
    return !hal_i2c_read_buffer( LIS2DE12_I2C_ADD_H, reg, data, len );
}

int8_t getLIS2DE12_Device_ID( ) {
    uint8_t rbuffer;
    HAL_I2C_Mem_Read(&_hi2c1, LIS2DE12_I2C_ADD_H, LIS2DE12_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &rbuffer, 1, 5000);
    if( rbuffer != LIS2DE12_ID ) {
        HAL_DBG_TRACE_INFO("Getting LIS2DE12 device ID... ");
        HAL_DBG_TRACE_ERROR("Failed to get LIS2DE12 device ID\r\n");
        return -1;
    }
    HAL_DBG_TRACE_INFO_VALUE("DONE\r\nINFO : Getting LIS2DE12 device ID... 0x%02X\r\n", rbuffer);
    return 0;
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
    // return !hal_i2c_write_buffer( 1, LIS2DE12_I2C_ADD_H, reg, data, len );

    //use HAL_I2C_Mem_Write instead of i2c_write_buffer
    return HAL_I2C_Mem_Write(&_hi2c1, LIS2DE12_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, data, len, 2000u);

    // if( i2c_write_buffer( id, device_addr, addr, buffer, size ) == SMTC_FAIL ) {
    //     // if first attempt fails due to an IRQ, try a second time
    //     if( i2c_write_buffer( id, device_addr, addr, buffer, size ) == SMTC_FAIL ) {
    //         return SMTC_FAIL;
    //     } else {
    //         return SMTC_SUCCESS;
    //     }
    // } else {
    //     return SMTC_SUCCESS;
    // }
}

int32_t setLIS2DE12_Block_Data_Update( uint8_t val ) {
    HAL_DBG_TRACE_INFO("Setting block data update... ");

    lis2de12_ctrl_reg4_t ctrl_reg4;
    if( lis2de12_read_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 ) == 0 ) {
        ctrl_reg4.bdu = 1;
        if ( lis2de12_write_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set block data update\r\n");
    return -1;
}

int32_t setLIS2DE12_Fifo_Mode( lis2de12_fm_t val ) {
    HAL_DBG_TRACE_INFO("Setting FIFO mode... ");

    lis2de12_fifo_ctrl_reg_t fifo_ctrl_reg;
    if( lis2de12_read_reg( LIS2DE12_FIFO_CTRL_REG, ( uint8_t* ) &fifo_ctrl_reg, 1 ) == 0 ) {
        fifo_ctrl_reg.fm = ( uint8_t ) val;
        if ( lis2de12_write_reg( LIS2DE12_FIFO_CTRL_REG, ( uint8_t* ) &fifo_ctrl_reg, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set FIFO mode\r\n");
    return -1;
}

int32_t setLIS2DE12_Full_Scale( lis2de12_fs_t val ) {
    HAL_DBG_TRACE_INFO("Setting full scale... ");

    lis2de12_ctrl_reg4_t ctrl_reg4;
    if( lis2de12_read_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 ) == 0 ) {
        ctrl_reg4.fs = ( uint8_t ) val;
        if ( lis2de12_write_reg( LIS2DE12_CTRL_REG4, ( uint8_t* ) &ctrl_reg4, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set full scale\r\n");
    return -1;
}

int32_t setLIS2DE12_High_Pass_Int_Conf( lis2de12_hp_t val ) {
    HAL_DBG_TRACE_INFO("Setting high pass filter... ");

    lis2de12_ctrl_reg2_t ctrl_reg2;
    if( lis2de12_read_reg( LIS2DE12_CTRL_REG2, ( uint8_t* ) &ctrl_reg2, 1 ) == 0 ) {
        ctrl_reg2.hp = ( uint8_t ) val;
        if ( lis2de12_write_reg( LIS2DE12_CTRL_REG2, ( uint8_t* ) &ctrl_reg2, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set high pass filter\r\n");
    return -1;
}

int32_t lis2de12_pin_int1_config_set( lis2de12_ctrl_reg3_t* val ) {
    HAL_DBG_TRACE_INFO("Setting pin int1 config... ");
    if (lis2de12_write_reg( LIS2DE12_CTRL_REG3, ( uint8_t* ) val, 1 ) == 0) {
        HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
        return 0;
    }
    HAL_DBG_TRACE_ERROR("Failed to set pin int1 config\r\n");
    return -1;
}

int32_t lis2de12_int1_pin_notification_mode_set( lis2de12_lir_int1_t val ) {
    HAL_DBG_TRACE_INFO("Setting int1 pin notification mode... ");
    lis2de12_ctrl_reg5_t ctrl_reg5;

    if( lis2de12_read_reg( LIS2DE12_CTRL_REG5, ( uint8_t* ) &ctrl_reg5, 1 ) == 0 ) {
        ctrl_reg5.lir_int1 = ( uint8_t ) val;
        if ( lis2de12_write_reg( LIS2DE12_CTRL_REG5, ( uint8_t* ) &ctrl_reg5, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set int1 pin notification mode\r\n");
    return -1;
}

int32_t lis2de12_int1_gen_conf_set( lis2de12_int1_cfg_t* val ) {
    HAL_DBG_TRACE_INFO("Setting int1 gen conf... ");
    if (lis2de12_write_reg( LIS2DE12_INT1_CFG, ( uint8_t* ) val, 1 ) == 0) {
        HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
        return 0;
    }
    HAL_DBG_TRACE_ERROR("Failed to set int1 gen conf\r\n");
    return -1;
}

int32_t lis2de12_int1_gen_threshold_set( uint8_t val ) {
    HAL_DBG_TRACE_INFO("Setting int1 gen threshold... ");

    lis2de12_int1_ths_t int1_ths;
    if( lis2de12_read_reg( LIS2DE12_INT1_THS, ( uint8_t* ) &int1_ths, 1 ) == 0 ) {
        int1_ths.ths = val;
        if ( lis2de12_write_reg( LIS2DE12_INT1_THS, ( uint8_t* ) &int1_ths, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set int1 gen threshold\r\n");
    return -1;
}

int32_t lis2de12_int1_gen_duration_set( uint8_t val ) {
    HAL_DBG_TRACE_INFO("Setting int1 gen duration... ");

    lis2de12_int1_duration_t int1_duration;
    if( lis2de12_read_reg( LIS2DE12_INT1_DURATION, ( uint8_t* ) &int1_duration, 1 ) == 0 ) {
        int1_duration.d = val;
        if ( lis2de12_write_reg( LIS2DE12_INT1_DURATION, ( uint8_t* ) &int1_duration, 1 ) == 0 ) {
            HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
            return 0;
        }
    }
    HAL_DBG_TRACE_ERROR("Failed to set int1 gen duration\r\n");
    return -1;
}



int32_t checkLIS2DE12_Temperature_Data_is_Ready( ) {
    HAL_DBG_TRACE_INFO("Checking if temperature data is ready... ");

    lis2de12_status_reg_aux_t status_reg_aux;

    if ( lis2de12_read_reg( LIS2DE12_STATUS_REG_AUX, ( uint8_t* ) &status_reg_aux, 1 ) != 0 ) {
        HAL_DBG_TRACE_ERROR("Failed to check if temperature data is ready\r\n");
        return -1;
    }

    if ( status_reg_aux.tda == 1 && status_reg_aux.tor == 0) {
        HAL_DBG_TRACE_INFO_VALUE("READY\r\n");
        return 0;
    }
    HAL_DBG_TRACE_WARNING("NOT READY (STATUS_REG_AUX: 0x%02X)\r\n", status_reg_aux);
    return -1;
}

int32_t enableLIS2DE12_Temperature_Sensor( ) {
    HAL_DBG_TRACE_INFO("Enabling LIS2DE12 internal temperature sensor... ");
    // uint8_t rbuffer = 0;
    // if (lis2de12_read_reg( LIS2DE12_TEMP_CFG_REG, ( uint8_t* ) &rbuffer, 1 ) != 0 ) {
    //     HAL_DBG_TRACE_ERROR("Failed to enable LIS2DE12 internal temperature sensor\r\n");
    //     return -1;
    // }
    // HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
    // HAL_DBG_TRACE_INFO_VALUE("rbuffer: 0x%02X\r\n", rbuffer);

    uint8_t buffer = 0xC0;
    if ( lis2de12_write_reg( LIS2DE12_TEMP_CFG_REG, ( uint8_t* ) &buffer, 1 ) != 0 ) {
        HAL_DBG_TRACE_ERROR("Failed to enable LIS2DE12 internal temperature sensor\r\n");
        return -1;
    }
    HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
}

int32_t setLIS2DE12_Data_Rate( ) {
    HAL_DBG_TRACE_INFO("Setting data rate... ");
    // lis2de12_ctrl_reg1_t ctrl_reg1;

    // if( lis2de12_read_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 ) == 0 ) {
    //     ctrl_reg1.lpen = 1U;
    //     ctrl_reg1.odr  = ( uint8_t ) val;
    //     if( lis2de12_write_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 ) == 0 ) {
    //         HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
    //         return 0;
    //     }
    // }
    // HAL_DBG_TRACE_ERROR("Failed to set data rate\r\n");
    // return -1;


    uint8_t buffer = 0x18;  //0b00011000
    if ( lis2de12_write_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &buffer, 1 ) != 0 ) {
        HAL_DBG_TRACE_ERROR("Failed to set data rate\r\n");
        return -1;
    }
    HAL_DBG_TRACE_INFO_VALUE("DONE\r\n");
}

int32_t lis2de12_temperature_raw_get( uint16_t* raw_temp ) {
    HAL_DBG_TRACE_INFO("Getting raw temperature...");

    uint8_t buf_tmp;
    if( lis2de12_read_reg( LIS2DE12_OUT_TEMP_L, &buf_tmp, 1 ) != 0 ) {
        HAL_DBG_TRACE_ERROR("Failed to get raw temperature\r\n");
        return -1;
    }
    *raw_temp = buf_tmp;

    HAL_DBG_TRACE_INFO_VALUE("%d\r\n", *raw_temp);

    if( lis2de12_read_reg( LIS2DE12_OUT_TEMP_H, &buf_tmp, 1 ) != 0 ) {
        HAL_DBG_TRACE_ERROR("Failed to get raw temperature\r\n");
        return -1;
    }
    *raw_temp += buf_tmp << 8;

    HAL_DBG_TRACE_INFO_VALUE("%d\r\n", *raw_temp);

    return 0;
}

int16_t acc_get_temperature( void ) {
    uint16_t temperature;

    if (checkLIS2DE12_Temperature_Data_is_Ready( ) == 0) {
        lis2de12_temperature_raw_get( &temperature );
    }

    /* Build the raw tmp */
    return ( int16_t ) temperature;
}

void initLIS2DE12(I2C_HandleTypeDef hi2c1) {
    HAL_DBG_TRACE_INFO("Initilizing LIS2DE12... ");

    _hi2c1 = hi2c1;

    if (getLIS2DE12_Device_ID() != 0) {
        HAL_DBG_TRACE_ERROR("Failed to initilize LIS2DE12\r\n");
        return;
    }

    enableLIS2DE12_Temperature_Sensor( );

    /* Set Output Data Rate to 1Hz */
    setLIS2DE12_Data_Rate( );

    /* Enable Block Data Update */
    setLIS2DE12_Block_Data_Update( PROPERTY_ENABLE );

    // /* Enable bypass mode */
    // setLIS2DE12_Fifo_Mode( LIS2DE12_BYPASS_MODE );


    

    // /* Set full scale to 2g */
    // setLIS2DE12_Full_Scale( LIS2DE12_2g );

    // /* Motion detection setup */
    // lis2de12_ctrl_reg1_t ctrl_reg1;
    // lis2de12_read_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 );
    // ctrl_reg1.xen  = 1;
    // ctrl_reg1.yen  = 1;
    // ctrl_reg1.zen  = 1;
    // ctrl_reg1.lpen = 1;
    // lis2de12_write_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 );

    // setLIS2DE12_High_Pass_Int_Conf( LIS2DE12_ON_INT1_GEN );

    // lis2de12_ctrl_reg3_t ctrl_reg3;
    // ctrl_reg3.i1_zyxda    = 0;
    // ctrl_reg3.i1_ia1      = 1;
    // ctrl_reg3.i1_ia2      = 0;
    // ctrl_reg3.i1_click    = 0;
    // ctrl_reg3.i1_overrun  = 0;
    // ctrl_reg3.i1_wtm      = 0;
    // ctrl_reg3.not_used_01 = 0;
    // ctrl_reg3.not_used_02 = 0;
    // lis2de12_pin_int1_config_set( &ctrl_reg3 );

    // lis2de12_int1_pin_notification_mode_set( LIS2DE12_INT1_LATCHED );

    // lis2de12_int1_cfg_t  lis2de12_int1_cfg;
    // lis2de12_int1_cfg.xhie = 1;
    // lis2de12_int1_cfg.yhie = 1;
    // lis2de12_int1_cfg.zhie = 1;
    // lis2de12_int1_gen_conf_set( &lis2de12_int1_cfg );

    // lis2de12_int1_gen_threshold_set( 4 );

    // lis2de12_int1_gen_duration_set( 3 );
}