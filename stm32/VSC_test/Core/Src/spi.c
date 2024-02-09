/**
  ******************************************************************************
  * @file           : spi.c
  * @brief          : Containing all SPI code
  ******************************************************************************
  */

#include "spi.h"

#include "main.h"

#include "configuration.h"

#include "lr1110_modem_board.h"     // for HAL_DBG_TRACE_ERROR

lr1110_gpio_status_t waitForGPIOState( gpio_t gpio, GPIO_PinState state, uint32_t timeout_ms ) {

    uint32_t start = hal_rtc_get_time_ms( );

    if( state == GPIO_PIN_RESET ) {
        while( HAL_GPIO_ReadPin( gpio.port, gpio.pin ) == GPIO_PIN_SET ) {
            if( ( int32_t )( hal_rtc_get_time_ms() - start ) > ( int32_t ) timeout_ms ) {
                HAL_DBG_TRACE_ERROR("\r\nTimeout occured while waiting for pin %d on port %d to become LOW\r\n", gpio.pin, gpio.port);
                return LR1110_GPIO_STATUS_TIMEOUT;
            }
        };
    } else {
        while( HAL_GPIO_ReadPin( gpio.port, gpio.pin ) == GPIO_PIN_RESET ) {
            if( ( int32_t )( hal_rtc_get_time_ms() - start ) > ( int32_t ) timeout_ms ) {
                HAL_DBG_TRACE_ERROR("\r\nTimeout occured while waiting for pin %d on port %d to become HIGH\r\n", gpio.pin, gpio.port);
                return LR1110_GPIO_STATUS_TIMEOUT;
            }
        };
    }
    return LR1110_GPIO_STATUS_OK;
}

lr1110_spi_status_t lr1110_spi_write( SPI_TypeDef* spi, const uint8_t* buffer, uint16_t length, uint32_t timeout_ms ) {

    lr1110_spi_status_t status = LR1110_SPI_STATUS_OK;

    for( uint16_t i = 0; i < length; i++ ) {

        uint32_t start = hal_rtc_get_time_ms( );

        turnOnLED(GPIOC, TX_LED_Pin);
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( int32_t )( hal_rtc_get_time_ms() - start ) > ( int32_t ) timeout_ms ) {
                HAL_DBG_TRACE_ERROR("\r\nTimeout occured while waiting for SPI to become ready for transmission\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        LL_SPI_TransmitData8( spi, buffer[i] );
        turnOffLED(GPIOC, TX_LED_Pin);

        turnOnLED(GPIOC, RX_LED_Pin);
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( int32_t )( hal_rtc_get_time_ms() - start ) > ( int32_t ) timeout_ms ) {
                HAL_DBG_TRACE_ERROR("\r\nTimeout occured while waiting for SPI to become ready for reception\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        LL_SPI_ReceiveData8( spi );
        turnOffLED(GPIOC, RX_LED_Pin);
    }
    return status;
}

lr1110_spi_status_t lr1110_spi_read_with_dummy_byte( SPI_TypeDef* spi, uint8_t* buffer, uint16_t length, uint8_t dummy_byte, uint32_t timeout_ms ) {

    lr1110_spi_status_t status = LR1110_SPI_STATUS_OK;

    for( uint16_t i = 0; i < length; i++ ) {

        uint32_t start = hal_rtc_get_time_ms( );

        turnOnLED(GPIOC, TX_LED_Pin);
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( int32_t )( hal_rtc_get_time_ms() - start ) > ( int32_t ) timeout_ms ) {
                HAL_DBG_TRACE_ERROR("\r\nTimeout occured while waiting for SPI to become ready for transmission\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        LL_SPI_TransmitData8( spi, dummy_byte );
        turnOffLED(GPIOC, TX_LED_Pin);

        turnOnLED(GPIOC, RX_LED_Pin);
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( int32_t )( hal_rtc_get_time_ms() - start ) > ( int32_t ) timeout_ms ) {
                HAL_DBG_TRACE_ERROR("\r\nTimeout occured while waiting for SPI to become ready for reception\r\n");
                status = LR1110_SPI_STATUS_TIMEOUT;
                break;
            }
        };
        buffer[i] = LL_SPI_ReceiveData8( spi );
        turnOffLED(GPIOC, RX_LED_Pin);
    }
    return status;
}

lr1110_spi_status_t lr1110_spi_read( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length ) {

    radio_t* radio_local = ( radio_t* ) context;
    uint8_t  dummy_byte  = 0x00;

    if (waitForGPIOState( radio_local->busy, GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    /* 1st SPI transaction */
    HAL_GPIO_WritePin( radio_local->nss.port, radio_local->nss.pin, GPIO_PIN_RESET );
    if (lr1110_spi_write( radio_local->spi, cbuffer, cbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio_local->nss.port, radio_local->nss.pin, GPIO_PIN_SET );

    if (waitForGPIOState( radio_local->busy, GPIO_PIN_RESET, 1000 ) != LR1110_GPIO_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    /* 2nd SPI transaction */
    HAL_GPIO_WritePin( radio_local->nss.port, radio_local->nss.pin, GPIO_PIN_RESET );
    if (lr1110_spi_write( radio_local->spi, &dummy_byte, 1, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    if (lr1110_spi_read_with_dummy_byte( radio_local->spi, rbuffer, rbuffer_length, LR1110_NOP, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    HAL_GPIO_WritePin( radio_local->nss.port, radio_local->nss.pin, GPIO_PIN_SET );

    return LR1110_SPI_STATUS_OK;
}

