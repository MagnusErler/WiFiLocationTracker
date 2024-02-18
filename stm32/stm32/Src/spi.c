/**
  ******************************************************************************
  * @file           : spi.c
  * @brief          : Containing all SPI code
  ******************************************************************************
  */

#include "spi.h"

#include "main.h"       // for HAL_DBG_TRACE-functions

radio_t* radio;

lr1110_gpio_status_t _waitForBusyState( GPIO_PinState state, uint32_t timeout_ms ) {

    uint32_t start = HAL_GetTick();

    if( state == GPIO_PIN_RESET ) {
        while( HAL_GPIO_ReadPin( radio->busy.port, radio->busy.pin ) == GPIO_PIN_SET ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("Timeout occured while waiting for BUSY to become LOW\r\n");
                return LR1110_GPIO_STATUS_TIMEOUT;
            }
        };
    } else {
        while( HAL_GPIO_ReadPin( radio->busy.port, radio->busy.pin ) == GPIO_PIN_RESET ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("Timeout occured while waiting for BUSY to become HIGH\r\n");
                return LR1110_GPIO_STATUS_TIMEOUT;
            }
        };
    }
    return LR1110_GPIO_STATUS_OK;
}

lr1110_spi_status_t _lr1110_spi_write( SPI_TypeDef* spi, const uint8_t* cbuffer, uint16_t cbuffer_length, uint32_t timeout_ms ) {

    lr1110_spi_status_t status = LR1110_SPI_STATUS_OK;

    for( uint16_t i = 0; i < cbuffer_length; i++ ) {

        uint32_t start = HAL_GetTick();

        turnOnLED(GPIOC, TX_LED_Pin);
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Timeout occured while waiting for SPI to become ready for transmission\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        LL_SPI_TransmitData8( spi, cbuffer[i] );
        turnOffLED(GPIOC, TX_LED_Pin);

        turnOnLED(GPIOC, RX_LED_Pin);
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Timeout occured while waiting for SPI to become ready for reception\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        LL_SPI_ReceiveData8( spi );
        turnOffLED(GPIOC, RX_LED_Pin);
    }
    return status;
}

lr1110_spi_status_t _lr1110_spi_read_with_dummy_byte( SPI_TypeDef* spi, uint8_t* cbuffer, uint16_t cbuffer_length, uint8_t dummy_byte, uint32_t timeout_ms ) {

    lr1110_spi_status_t status = LR1110_SPI_STATUS_OK;

    for( uint16_t i = 0; i < cbuffer_length; i++ ) {

        uint32_t start = HAL_GetTick( );

        turnOnLED(GPIOC, TX_LED_Pin);
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_read_with_dummy_byte(): Timeout occured while waiting for SPI to become ready for transmission\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        LL_SPI_TransmitData8( spi, dummy_byte );
        turnOffLED(GPIOC, TX_LED_Pin);

        turnOnLED(GPIOC, RX_LED_Pin);
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_read_with_dummy_byte(): Timeout occured while waiting for SPI to become ready for reception\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        cbuffer[i] = LL_SPI_ReceiveData8( spi );
        turnOffLED(GPIOC, RX_LED_Pin);
    }
    return status;
}

lr1110_spi_status_t lr1110_spi_read( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length ) {

    radio = ( radio_t* ) context;
    uint8_t  dummy_byte  = 0x00;

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    // Start of 1st SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if (_lr1110_spi_write( radio->spi, cbuffer, cbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // End of 1st SPI transaction

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    // Start of 2nd SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if (_lr1110_spi_write( radio->spi, &dummy_byte, 1, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    if (_lr1110_spi_read_with_dummy_byte( radio->spi, rbuffer, rbuffer_length, LR1110_NOP, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // End of 2nd SPI transaction

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t lr1110_spi_write( const void* context, const uint8_t* cbuffer, uint16_t cbuffer_length ) {

    lr1110_spi_status_t status = LR1110_SPI_STATUS_OK;

    radio = ( radio_t* ) context;

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    // Start of SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if (_lr1110_spi_write( radio->spi, cbuffer, cbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // End of SPI transaction

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    
    return status;
}
