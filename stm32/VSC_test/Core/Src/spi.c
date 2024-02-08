/**
  ******************************************************************************
  * @file           : spi.c
  * @brief          : Containing all SPI code
  ******************************************************************************
  */

#include "spi.h"

#include "main.h"

#include "configuration.h"

#include <stdlib.h>

lr1110_spi_status_t lr1110_spi_read( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length ) {

  radio_t* radio_local = ( radio_t* ) context;
  uint8_t  dummy_byte  = 0x00;

  lr1110_gpio_wait_for_state( radio_local->busy, LR1110_SPI_GPIO_PIN_STATE_LOW );

  /* 1st SPI transaction */
  lr1110_gpio_set_pin_state( radio_local->nss, LR1110_SPI_GPIO_PIN_STATE_LOW );
  system_spi_write( radio_local->spi, cbuffer, cbuffer_length );
  lr1110_gpio_set_pin_state( radio_local->nss, LR1110_SPI_GPIO_PIN_STATE_HIGH );

  system_gpio_wait_for_state( radio_local->busy, LR1110_SPI_GPIO_PIN_STATE_LOW );

  /* 2nd SPI transaction */
  lr1110_gpio_set_pin_state( radio_local->nss, LR1110_SPI_GPIO_PIN_STATE_LOW );
  system_spi_write( radio_local->spi, &dummy_byte, 1 );
  lr1110_spi_read_with_dummy_byte( radio_local->spi, rbuffer, rbuffer_length, LR1110_NOP );
  lr1110_gpio_set_pin_state( radio_local->nss, LR1110_SPI_GPIO_PIN_STATE_HIGH );

  return LR1110_SPI_STATUS_OK;
}

void lr1110_gpio_wait_for_state( gpio_t gpio, lr1110_spi_gpio_pin_state_t state ) {
  if( state == LR1110_SPI_GPIO_PIN_STATE_LOW )
  {
      while( LL_GPIO_IsInputPinSet( gpio.port, gpio.pin ) )
      {
      };
  }
  else
  {
      while( !LL_GPIO_IsInputPinSet( gpio.port, gpio.pin ) )
      {
      };
  }
}

void lr1110_gpio_set_pin_state( gpio_t gpio, const lr1110_spi_gpio_pin_state_t state )
{
    switch( state )
    {
    case LR1110_SPI_GPIO_PIN_STATE_LOW:
        LL_GPIO_ResetOutputPin( gpio.port, gpio.pin );
        break;
    case LR1110_SPI_GPIO_PIN_STATE_HIGH:
        LL_GPIO_SetOutputPin( gpio.port, gpio.pin );
        break;
    default:
        break;
    }
}

void lr1110_spi_read_with_dummy_byte( SPI_TypeDef* spi, uint8_t* buffer, uint16_t length, uint8_t dummy_byte )
{
    for( uint16_t i = 0; i < length; i++ )
    {
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 )
        {
        };

        LL_SPI_TransmitData8( spi, dummy_byte );

        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 )
        {
        };

        buffer[i] = LL_SPI_ReceiveData8( spi );
    }
}