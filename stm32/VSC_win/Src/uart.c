
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_utils.h"
//#include "lr1110_modem_board.h"
#include "smtc_hal.h"

#include "configuration.h"

#if( HAL_DBG_TRACE == HAL_FEATURE_ON )
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif

void hal_mcu_trace_print( const char* fmt, ... )
{
#if HAL_DBG_TRACE == HAL_FEATURE_ON
    va_list argp;
    va_start( argp, fmt );

    char string[255];
    if( 0 < vsprintf( string, fmt, argp ) )  // build string
    {
        hal_uart_tx( 2, ( uint8_t* ) string, strlen( string ) );
    }

    va_end( argp );
#endif
}