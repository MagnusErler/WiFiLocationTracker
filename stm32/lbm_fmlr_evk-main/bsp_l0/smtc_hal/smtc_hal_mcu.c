/*!
 * \file      smtc_hal_mcu.c
 *
 * \brief     MCU Hardware Abstraction Layer implementation
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "smtc_hal_mcu.h"
#include "modem_pinout.h"

#include "stm32l0xx_hal.h"
#include "stm32l0xx_ll_utils.h"

#include "smtc_hal_uart.h"
#include "smtc_hal_rtc.h"
#include "smtc_hal_spi.h"
#include "smtc_hal_lp_timer.h"
#include "smtc_hal_rng.h"

#include "timer.h"

#if( MODEM_HAL_DBG_TRACE == MODEM_HAL_FEATURE_ON )
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

// Low power is enabled (0 will disable it)
#define LOW_POWER_MODE 1
// 1 to enable debug with probe (ie do not de init pins)
#define HW_DEBUG_PROBE 0

/*!
 * Watchdog counter reload value during sleep
 *
 * \remark The period must be lower than MCU watchdog period
 */
#define WATCHDOG_RELOAD_PERIOD_SECONDS 20
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */
// Low Power options
typedef enum low_power_mode_e {
    LOW_POWER_ENABLE,
    LOW_POWER_DISABLE,
    LOW_POWER_DISABLE_ONCE
} low_power_mode_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */
static volatile bool             exit_wait       = false;
static volatile low_power_mode_t lp_current_mode = LOW_POWER_ENABLE;

#if( LOW_POWER_MODE == 1 )
static TimerEvent_t wut;
static volatile bool wut_flag;
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */
static void system_clock_config( void );
static void mcu_gpio_init( void );

#if( LOW_POWER_MODE == 1 )
static void lpm_mcu_reinit( void );
static void lpm_enter_stop_mode( void );
static void lpm_exit_stop_mode( void );
static void wut_cb( void* context );
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */


void hal_mcu_init( void ) {
    HAL_Init( );             // Initialize MCU HAL library
    system_clock_config( );  // Initialize clocks
    mcu_gpio_init( );        // Initialize GPIOs

    uart1_init( );

    hal_spi_init( RADIO_SPI_ID, RADIO_SPI_MOSI, RADIO_SPI_MISO, RADIO_SPI_SCLK );
    hal_rtc_init( );

    //hal_watchdog_init( );
}

void hal_mcu_reset( void ) {
    __disable_irq( );
    NVIC_SystemReset( );  // Restart system
}

void __attribute__( ( optimize( "O0" ) ) ) hal_mcu_wait_us( const int32_t microseconds ) {
    // Work @80MHz
    const uint32_t nb_nop = microseconds * 1000 / 137;

    for( uint32_t i = 0; i < nb_nop; i++ ) {
        __NOP( );
    }
}



void SysTick_Handler( void ) {
    HAL_IncTick( );
    HAL_SYSTICK_IRQHandler( );
}

void HAL_MspInit( void ) {
    // System interrupt init
    // HAL_NVIC_SetPriority( MemoryManagement_IRQn, 0, 0 );
    // HAL_NVIC_SetPriority( BusFault_IRQn, 0, 0 );
    // HAL_NVIC_SetPriority( UsageFault_IRQn, 0, 0 );
    // HAL_NVIC_SetPriority( SVCall_IRQn, 0, 0 );
    // HAL_NVIC_SetPriority( DebugMonitor_IRQn, 0, 0 );
    // HAL_NVIC_SetPriority( PendSV_IRQn, 0, 0 );
    HAL_NVIC_SetPriority( SysTick_IRQn, 0, 0 );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void system_clock_config( void ) {
    // Configure the main internal regulator output voltage
    RCC_OscInitTypeDef       rcc_osc_init_struct = { 0 };
    RCC_ClkInitTypeDef       rcc_clk_init_struct = { 0 };
    RCC_PeriphCLKInitTypeDef periph_clk_init     = { 0 };

    // Set low drive on LSE to reduce conso
    HAL_PWR_EnableBkUpAccess( );
    __HAL_RCC_LSEDRIVE_CONFIG( RCC_LSEDRIVE_LOW );

    // HSI is enabled after System reset, activate PLL with HSI as source
    rcc_osc_init_struct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    rcc_osc_init_struct.LSEState            = RCC_LSE_ON;
    rcc_osc_init_struct.HSIState            = RCC_HSI_ON;
    rcc_osc_init_struct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    rcc_osc_init_struct.LSIState            = RCC_LSI_ON;
    rcc_osc_init_struct.PLL.PLLState        = RCC_PLL_ON;
    rcc_osc_init_struct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    rcc_osc_init_struct.PLL.PLLMUL          = RCC_PLL_MUL6;
    rcc_osc_init_struct.PLL.PLLDIV          = RCC_PLL_DIV3;

    if( HAL_RCC_OscConfig( &rcc_osc_init_struct ) != HAL_OK ) {
        mcu_panic( );  // Initialization Error
    }

    // Initializes the CPU, AHB and APB busses clocks
    rcc_clk_init_struct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    rcc_clk_init_struct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk_init_struct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    rcc_clk_init_struct.APB1CLKDivider = RCC_HCLK_DIV1;
    rcc_clk_init_struct.APB2CLKDivider = RCC_HCLK_DIV1;

    if( HAL_RCC_ClockConfig( &rcc_clk_init_struct, FLASH_LATENCY_1 ) != HAL_OK ) {
        mcu_panic( );  // Initialization Error
    }

    periph_clk_init.PeriphClockSelection =
        RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_LPTIM1;
    periph_clk_init.RTCClockSelection       = RCC_RTCCLKSOURCE_LSE;
    periph_clk_init.LptimClockSelection    = RCC_LPTIM1CLKSOURCE_LSE;

#if( BSP_USE_PRINTF_UART == BSP_FEATURE_ON )
    periph_clk_init.PeriphClockSelection |= RCC_PERIPHCLK_USART1;
    periph_clk_init.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
#endif
#if( BSP_USE_USER_UART == BSP_FEATURE_ON )
    periph_clk_init.PeriphClockSelection |= RCC_PERIPHCLK_USART2;
    periph_clk_init.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
#endif
#ifdef I2C1_ENABLED
    periph_clk_init.PeriphClockSelection |= RCC_PERIPHCLK_I2C1;
    periph_clk_init.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
#endif

    if( HAL_RCCEx_PeriphCLKConfig( &periph_clk_init ) != HAL_OK ) {
        mcu_panic( );  // Initialization Error
    }

    // Enable Power Clock
    __HAL_RCC_PWR_CLK_ENABLE( );

    // Ensure that HSI is wake-up system clock
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG( RCC_STOP_WAKEUPCLOCK_HSI );
}

static void mcu_gpio_init( void ) {
#if( HW_DEBUG_PROBE == 1 )
    // Enable debug in sleep/stop/standby
    HAL_DBGMCU_EnableDBGSleepMode( );
    HAL_DBGMCU_EnableDBGStopMode( );
    HAL_DBGMCU_EnableDBGStandbyMode( );
#endif

    hal_gpio_init_out( RADIO_NSS, 1 );
    hal_gpio_init_in( RADIO_BUSY_PIN, BSP_GPIO_PULL_MODE_NONE, BSP_GPIO_IRQ_MODE_OFF, NULL );
    // Here init only the pin as an exti rising and the callback will be attached later
    hal_gpio_init_in( RADIO_DIOX, BSP_GPIO_PULL_MODE_DOWN, BSP_GPIO_IRQ_MODE_RISING, NULL );
    hal_gpio_init_out( RADIO_NRST, 1 );
#if defined( SX128X )
    hal_gpio_init_out( RADIO_ANTENNA_SWITCH, 1 );
#elif defined( LR11XX_TRANSCEIVER ) && defined( ENABLE_MODEM_GNSS_FEATURE )
    hal_gpio_init_out( RADIO_LNA_CTRL, 0 );
#elif defined( SX126X )
    // If the sx126x drives the rf switch with dio2, just put the SX126X_RADIO_RF_SWITCH_CTRL in pull up
    hal_gpio_init_in( SX126X_RADIO_RF_SWITCH_CTRL, BSP_GPIO_PULL_MODE_UP, BSP_GPIO_IRQ_MODE_OFF, NULL );
#endif
}

#if( LOW_POWER_MODE == 1 )

/**
 * @brief Enters Low Power Stop Mode
 *
 * @note ARM exits the function when waking up
 *
 */
static void lpm_enter_stop_mode( void ) {
    CRITICAL_SECTION_BEGIN( );

    // Deinit periph & enter Stop Mode
    lpm_mcu_deinit( );
    HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );

    CRITICAL_SECTION_END( );
}

/**
 * @brief Exists Low Power Stop Mode
 *
 */
static void lpm_exit_stop_mode( void ) {
    // Disable IRQ while the MCU is not running on HSI
    CRITICAL_SECTION_BEGIN( );

    // Initializes the peripherals
    lpm_mcu_reinit( );

    CRITICAL_SECTION_END( );
}


/**
 * @brief Re-init MCU clock after a wait in stop mode 2
 *
 */
static void lpm_mcu_reinit( void ) {
    RCC_ClkInitTypeDef rcc_clk_init_struct = { 0 };
    RCC_OscInitTypeDef rcc_osc_init_struct = { 0 };
    uint32_t           flash_latency       = 0;

    // Enable Power Control clock
    __HAL_RCC_PWR_CLK_ENABLE( );

    // Get the Oscillators configuration according to the internal RCC registers
    HAL_RCC_GetOscConfig( &rcc_osc_init_struct );

    // Enable PLL
    rcc_osc_init_struct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    rcc_osc_init_struct.PLL.PLLState   = RCC_PLL_ON;

    if( HAL_RCC_OscConfig( &rcc_osc_init_struct ) != HAL_OK ) {
        mcu_panic( );
    }

    // Get the Clocks configuration according to the internal RCC registers
    HAL_RCC_GetClockConfig( &rcc_clk_init_struct, &flash_latency );

    // Select PLL as system clock source and keep HCLK, PCLK1 and PCLK2 clocks dividers as before
    rcc_clk_init_struct.ClockType    = RCC_CLOCKTYPE_SYSCLK;
    rcc_clk_init_struct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

    if( HAL_RCC_ClockConfig( &rcc_clk_init_struct, flash_latency ) != HAL_OK ) {
        mcu_panic( );
    }

    // Initialize UART
#if( MODEM_HAL_DBG_TRACE == MODEM_HAL_FEATURE_ON )
    uart1_init( );
#endif
#if defined( HW_MODEM_ENABLED )
    uart4_init( );
#endif
#ifdef I2C1_ENABLED
    hal_i2c_init( 1, I2C1_SDA, I2C1_SCL );
#endif
#ifdef SPI1_ENABLED
    hal_spi_init( 1, SPI1_MOSI, SPI1_MISO, SPI1_SCK );
#endif

    // Initialize SPI
    hal_spi_init( RADIO_SPI_ID, RADIO_SPI_MOSI, RADIO_SPI_MISO, RADIO_SPI_SCLK );
}

static void wut_cb( void* context ) {
    wut_flag = true;
}

#endif

void hal_mcu_delay_ms( const uint32_t milliseconds ) {
    HAL_Delay( milliseconds );
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler( void ) {
    SMTC_HAL_TRACE_ERROR( "\x1B[0;31m" );  // red color
    SMTC_HAL_TRACE_ERROR( "HARDFAULT_Handler\n" );
    SMTC_HAL_TRACE_ERROR( "\x1B[0m" );  // default color

    while( 1 ) {
    }
}

/* --- EOF ------------------------------------------------------------------ */
