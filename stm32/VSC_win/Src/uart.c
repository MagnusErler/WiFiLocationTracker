
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_utils.h"

#include "smtc_hal_gpio_pin_names.h"



/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */
 
 /*!
 * @brief UART structure
 */
typedef struct hal_uart_s
{
    USART_TypeDef*     interface;
    UART_HandleTypeDef handle;
    struct
    {
        hal_gpio_pin_names_t tx;
        hal_gpio_pin_names_t rx;
    } pins;
} hal_uart_t;

static hal_uart_t hal_uart[] = {
    [0] =
        {
            .interface = USART1,
            .handle    = { NULL },
            .pins =
                {
                    .tx = NC,
                    .rx = NC,
                },
        },
    [1] =
        {
            .interface = USART2,
            .handle    = { NULL },
            .pins =
                {
                    .tx = NC,
                    .rx = NC,
                },
        },
    [2] =
        {
            .interface = USART3,
            .handle    = { NULL },
            .pins =
                {
                    .tx = NC,
                    .rx = NC,
                },
        },
};

void hal_uart_init( const uint32_t id, const hal_gpio_pin_names_t uart_tx, const hal_gpio_pin_names_t uart_rx )
{
    assert_param( ( id > 0 ) && ( ( id - 1 ) < sizeof( hal_uart ) ) );
    uint32_t local_id = id - 1;

    hal_uart[local_id].handle.Instance                    = hal_uart[local_id].interface;
    hal_uart[local_id].handle.Init.BaudRate               = 115200;
    hal_uart[local_id].handle.Init.WordLength             = UART_WORDLENGTH_8B;
    hal_uart[local_id].handle.Init.StopBits               = UART_STOPBITS_1;
    hal_uart[local_id].handle.Init.Parity                 = UART_PARITY_NONE;
    hal_uart[local_id].handle.Init.Mode                   = UART_MODE_TX_RX;
    hal_uart[local_id].handle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    hal_uart[local_id].handle.Init.OverSampling           = UART_OVERSAMPLING_16;
    hal_uart[local_id].handle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    hal_uart[local_id].handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    hal_uart[local_id].pins.tx = uart_tx;
    hal_uart[local_id].pins.rx = uart_rx;

    if( HAL_UART_Init( &hal_uart[local_id].handle ) != HAL_OK )
    {
        //hal_mcu_panic( );
    }
    __HAL_UART_ENABLE( &hal_uart[local_id].handle );
}

void hal_uart_tx( const uint32_t id, uint8_t* buff, uint16_t len ) {
    assert_param( ( id > 0 ) && ( ( id - 1 ) < sizeof( hal_uart ) ) );
    uint32_t local_id = id - 1;

    HAL_UART_Transmit( &hal_uart[local_id].handle, ( uint8_t* ) buff, len, 0xffffff );
}

void hal_mcu_trace_print( const char* fmt, ... ) {
    va_list argp;
    va_start( argp, fmt );

    char string[255];
    if( 0 < vsprintf( string, fmt, argp ) )  // build string
    {
        hal_uart_tx( 2, ( uint8_t* ) string, strlen( string ) );
    }

    va_end( argp );
}

