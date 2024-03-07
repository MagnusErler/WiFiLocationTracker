
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

    assert_param( ( id > 0 ) && ( ( id - 1 ) < sizeof( hal_i2c ) ) );
    uint32_t local_id = id - 1;

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
    HAL_DBG_TRACE_INFO("Set Data Rate\n");
    lis2de12_ctrl_reg1_t ctrl_reg1;
    int32_t              ret;

    ret = lis2de12_read_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 );
    if( ret == 0 ) {
        ctrl_reg1.lpen = 1U;
        ctrl_reg1.odr  = ( uint8_t ) val;
        ret            = lis2de12_write_reg( LIS2DE12_CTRL_REG1, ( uint8_t* ) &ctrl_reg1, 1 );
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
                return 0;
            }
        }
        i++;
    }
    HAL_DBG_TRACE_INFO_VALUE("DONE (Device ID: 0x%02X)\n", who_am_i);

    /* Set Output Data Rate to 10Hz */
    lis2de12_data_rate_set( LIS2DE12_ODR_10Hz );
}