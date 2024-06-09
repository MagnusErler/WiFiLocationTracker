/*!
 * @file      hall_effect.c
 *
 * @brief     Hall effect sensor driver implementation.
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

#include "hall_effect.h"
//#include "smtc_hal.h"
#include "modem_pinout.h"
#include "smtc_hal_gpio.h"
//#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type
#include <stddef.h>

#include "smtc_hal_dbg_trace.h"

//#include "lr1110_trk1xks_board.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Hardware INT IRQ callback initialization
 */
static void hall_effect1_irq_handler( void* obj );
static void hall_effect2_irq_handler( void* obj );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Hall Effect interrupt flag state
 */
static volatile bool hall_effect1_irq_state = false;
static volatile bool hall_effect2_irq_state = false;

/*!
 * @brief Hall Effect gpio irq definition
 */
hal_gpio_irq_t hall_effect1 = {
    .pin      = HALL_OUT1,
    .callback = hall_effect1_irq_handler,
    .context  = NULL,
};
hal_gpio_irq_t hall_effect2 = {
    .pin      = HALL_OUT2,
    .callback = hall_effect2_irq_handler,
    .context  = NULL,
};


/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */



void smtc_board_hall_effect_enable( bool enable )
{
    SMTC_HAL_TRACE_PRINTF( "hall effect sensor enable : %d\n", enable );

    if( enable == true )
    {
        hal_gpio_init_out( HALL_VCC, 0 );
        hal_gpio_set_value( HALL_VCC, 1 );
        hall_effect_init( HALL_EFFECT_IRQ_ON );
    }
    else
    {
        /* Stop Effect hall sensors while tracker is static */
        hall_effect_deinit( );
        hal_gpio_set_value( HALL_VCC, 0 );
        //hal_gpio_deinit( HALL_VCC );
    }
}










void hall_effect_init( bool irq_enable )
{
    if( irq_enable == true )
    {
        hal_gpio_init_in( hall_effect1.pin, BSP_GPIO_PULL_MODE_UP, BSP_GPIO_IRQ_MODE_FALLING, &hall_effect1 );
        hal_gpio_init_in( hall_effect2.pin, BSP_GPIO_PULL_MODE_UP, BSP_GPIO_IRQ_MODE_FALLING, &hall_effect2 );
    }
    else
    {
        hal_gpio_init_in( hall_effect1.pin, BSP_GPIO_PULL_MODE_UP, BSP_GPIO_IRQ_MODE_OFF, NULL );
        hal_gpio_init_in( hall_effect2.pin, BSP_GPIO_PULL_MODE_UP, BSP_GPIO_IRQ_MODE_OFF, NULL );
    }
}

void hall_effect_deinit( void )
{
    hal_gpio_irq_deatach( &hall_effect1 );
    hal_gpio_irq_deatach( &hall_effect2 );
    //hal_gpio_deinit( hall_effect.pin );
}

uint8_t read_hall_effect1_output( void ) { return hal_gpio_get_value( hall_effect1.pin ); }
uint8_t read_hall_effect2_output( void ) { return hal_gpio_get_value( hall_effect2.pin ); }

bool get_hall_effect1_irq_state( void ) { return hall_effect1_irq_state; }
bool get_hall_effect2_irq_state( void ) { return hall_effect2_irq_state; }

void clear_hall_effect1_irq_state( void )
{
    //leds_off( LED_RX_MASK );
    hall_effect1_irq_state = false;
}

void clear_hall_effect2_irq_state( void )
{
    //leds_off( LED_RX_MASK );
    hall_effect2_irq_state = false;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void hall_effect1_irq_handler( void* context )
{
    //leds_on( LED_RX_MASK );
    hall_effect1_irq_state = true;
}

static void hall_effect2_irq_handler( void* context )
{
    //leds_on( LED_RX_MASK );
    hall_effect2_irq_state = true;
}

/* --- EOF ------------------------------------------------------------------ */
