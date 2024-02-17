


#include <stdio.h>
#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type








#define HAL_DBG_TRACE_COLOR_BLACK "\x1B[0;30m"
#define HAL_DBG_TRACE_COLOR_RED "\x1B[0;31m"
#define HAL_DBG_TRACE_COLOR_GREEN "\x1B[0;32m"
#define HAL_DBG_TRACE_COLOR_YELLOW "\x1B[0;33m"
#define HAL_DBG_TRACE_COLOR_BLUE "\x1B[0;34m"
#define HAL_DBG_TRACE_COLOR_MAGENTA "\x1B[0;35m"
#define HAL_DBG_TRACE_COLOR_CYAN "\x1B[0;36m"
#define HAL_DBG_TRACE_COLOR_WHITE "\x1B[0;37m"
#define HAL_DBG_TRACE_COLOR_DEFAULT "\x1B[0m"

#if( UNIT_TEST_DBG )
#define HAL_DBG_TRACE_PRINTF( ... ) printf( __VA_ARGS__ )
#else
#define HAL_DBG_TRACE_PRINTF( ... ) hal_mcu_trace_print( __VA_ARGS__ )
#endif

#define HAL_DBG_TRACE_MSG( msg )                             \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
        HAL_DBG_TRACE_PRINTF( msg );                         \
    } while( 0 );

#define HAL_DBG_TRACE_MSG_COLOR( msg, color )                \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( color );                       \
        HAL_DBG_TRACE_PRINTF( msg );                         \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_INFO( ... )                            \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_GREEN );   \
        HAL_DBG_TRACE_PRINTF( "INFO : " );                   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_WARNING( ... )                         \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_YELLOW );  \
        HAL_DBG_TRACE_PRINTF( "WARN : " );                   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_ERROR( ... )                           \
    do                                                       \
    {                                                        \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_RED );     \
        HAL_DBG_TRACE_PRINTF( "ERROR: " );                   \
        HAL_DBG_TRACE_PRINTF( __VA_ARGS__ );                 \
        HAL_DBG_TRACE_PRINTF( HAL_DBG_TRACE_COLOR_DEFAULT ); \
    } while( 0 );

#define HAL_DBG_TRACE_ARRAY( msg, array, len )                                \
    do                                                                        \
    {                                                                         \
        HAL_DBG_TRACE_PRINTF( "%s - (%lu bytes):\n", msg, ( uint32_t ) len ); \
        for( uint32_t i = 0; i < ( uint32_t ) len; i++ )                      \
        {                                                                     \
            if( ( ( i % 16 ) == 0 ) && ( i > 0 ) )                            \
            {                                                                 \
                HAL_DBG_TRACE_PRINTF( "\n" );                                 \
            }                                                                 \
            HAL_DBG_TRACE_PRINTF( " %02X", array[i] );                        \
        }                                                                     \
        HAL_DBG_TRACE_PRINTF( "\n" );                                         \
    } while( 0 );

#define HAL_DBG_TRACE_PACKARRAY( msg, array, len )       \
    do                                                   \
    {                                                    \
        for( uint32_t i = 0; i < ( uint32_t ) len; i++ ) \
        {                                                \
            HAL_DBG_TRACE_PRINTF( "%02X", array[i] );    \
        }                                                \
    } while( 0 );

#define HAL_PERF_TEST_TRACE_PRINTF( ... )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief HAL Gpio pin name
 */
typedef enum hal_gpio_pin_names_e
{
    // GPIOA
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    // GPIOB
    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,
    // GPIOC
    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
    PC_7  = 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,
    // GPIOD
    PD_2 = 0x32,
    // GPIOH
    PH_0 = 0x70,
    PH_1 = 0x71,
    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,
    // Arduino interface names
    A0  = PA_0,
    A1  = PA_1,
    A2  = PA_4,
    A3  = PB_0,
    A4  = PC_1,
    A5  = PC_0,
    D0  = PA_3,
    D1  = PA_2,
    D2  = PA_10,
    D3  = PB_3,
    D4  = PB_5,
    D5  = PB_4,
    D6  = PB_10,
    D7  = PA_8,
    D8  = PA_9,
    D9  = PC_7,
    D10 = PB_6,
    D11 = PA_7,
    D12 = PA_6,
    D13 = PA_5,
    D14 = PB_9,
    D15 = PB_8,
    // MCU peripherals
    UART1_RX    = PA_10,
    UART1_TX    = PA_9,
    UART2_RX    = PA_3,
    UART2_TX    = PA_2,
    I2C_SCL     = PB_8,
    I2C_SDA     = PB_9,
    SPI1_MOSI   = PA_7,
    SPI1_MISO   = PA_6,
    SPI1_SCLK   = PA_5,
    PWM_OUT     = PB_3,
    OSC_LSE_IN  = PC_14,
    OSC_LSE_OUT = PC_15,
    OSC_HSE_IN  = PH_0,
    OSC_HSE_OUT = PH_1,
    SWCLK       = PA_14,
    SWDAT       = PA_13,
    // Not connected
    NC = -1
} hal_gpio_pin_names_t;








/*!
 * @brief Prints debug trace
 *
 * @param variadics arguments
 */
void hal_mcu_trace_print( const char* fmt, ... );

/*!
 * @brief Initializes UART
 *
 * @param id UART id
 * @param uart_tx UART TX pin
 * @param uart_rx UART RX pin
 */
void hal_uart_init( const uint32_t id, const hal_gpio_pin_names_t uart_tx, const hal_gpio_pin_names_t uart_rx );

