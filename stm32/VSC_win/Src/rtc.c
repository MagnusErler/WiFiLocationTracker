
 /*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <time.h>
#include <math.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_rtc.h"
#include "rtc.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/* sub-second number of bits */
#define N_PREDIV_S                     10U

/* Synchronous prediv */
#define PREDIV_S                       ( ( 1U << N_PREDIV_S ) - 1U )

/* Asynchronous prediv */
#define PREDIV_A                       ( ( 1U << ( 15U - N_PREDIV_S ) ) - 1U )

/* Sub-second mask definition */
#define ALARM_SUBSECOND_MASK           ( N_PREDIV_S << RTC_ALRMASSR_MASKSS_Pos )

/*!
 * @brief Days, Hours, Minutes and seconds
 */
#define DAYS_IN_LEAP_YEAR              ( ( uint32_t ) 366U )
#define DAYS_IN_YEAR                   ( ( uint32_t ) 365U )
#define SECONDS_IN_1DAY                ( ( uint32_t ) 86400U )
#define SECONDS_IN_1HOUR               ( ( uint32_t ) 3600U )
#define SECONDS_IN_1MINUTE             ( ( uint32_t ) 60U )
#define MINUTES_IN_1HOUR               ( ( uint32_t ) 60U )
#define HOURS_IN_1DAY                  ( ( uint32_t ) 24U )

/*!
 * @brief Correction factors
 */
#define DAYS_IN_MONTH_CORRECTION_NORM  ( ( uint32_t ) 0x99AAA0 )
#define DAYS_IN_MONTH_CORRECTION_LEAP  ( ( uint32_t ) 0x445550 )

/*!
 * @brief Calculates ceiling( X / N )
 */
#define DIVC( X, N )                   ( ( ( X ) + ( N ) -1 ) / ( N ) )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

hal_rtc_t hal_rtc;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief Get the elapsed time in seconds and milliseconds since RTC initialization
 *
 * @param [out] milliseconds Number of milliseconds elapsed since RTC
 *                           initialization
 * @returns seconds           Number of seconds elapsed since RTC initialization
 */
static uint32_t hal_rtc_get_calendar_time( uint16_t* milliseconds );

/*!
 * @brief Get current full resolution RTC timestamp in ticks
 *
 * @returns timestamp_in_ticks Current timestamp in ticks
 */
static uint64_t rtc_get_timestamp_in_ticks( RTC_DateTypeDef* date, RTC_TimeTypeDef* time );

uint32_t hal_rtc_get_time_ms( void )
{
    uint32_t seconds      = 0;
    uint16_t milliseconds = 0;

    seconds = hal_rtc_get_calendar_time( &milliseconds );

    return seconds * 1000 + milliseconds;
}

static uint32_t hal_rtc_get_calendar_time( uint16_t* milliseconds )
{
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    uint32_t        ticks;

    uint64_t timestamp_in_ticks = rtc_get_timestamp_in_ticks( &date, &time );

    uint32_t seconds = ( uint32_t )( timestamp_in_ticks >> N_PREDIV_S );

    ticks = ( uint32_t ) timestamp_in_ticks & PREDIV_S;

    uint32_t second    = ticks >> N_PREDIV_S;
    uint32_t local_tick = ticks & PREDIV_S;

    *milliseconds = ( second * 1000 ) + ( ( local_tick * 1000 ) >> N_PREDIV_S );

    return seconds;
}

static uint64_t rtc_get_timestamp_in_ticks( RTC_DateTypeDef* date, RTC_TimeTypeDef* time )
{
    uint64_t timestamp_in_ticks = 0;
    uint32_t correction;
    uint32_t seconds;

    /* Make sure it is correct due to asynchronous nature of RTC */
    volatile uint32_t ssr;

    do
    {
        ssr = RTC->SSR;
        HAL_RTC_GetDate( &hal_rtc.handle, date, RTC_FORMAT_BIN );
        HAL_RTC_GetTime( &hal_rtc.handle, time, RTC_FORMAT_BIN );
    } while( ssr != RTC->SSR );

    /* Calculate amount of elapsed days since 01/01/2000 */
    seconds = DIVC( ( DAYS_IN_YEAR * 3 + DAYS_IN_LEAP_YEAR ) * date->Year, 4 );

    correction = ( ( date->Year % 4 ) == 0 ) ? DAYS_IN_MONTH_CORRECTION_LEAP : DAYS_IN_MONTH_CORRECTION_NORM;

    seconds +=
        ( DIVC( ( date->Month - 1 ) * ( 30 + 31 ), 2 ) - ( ( ( correction >> ( ( date->Month - 1 ) * 2 ) ) & 0x03 ) ) );

    seconds += ( date->Date - 1 );

    /* Convert from days to seconds */
    seconds *= SECONDS_IN_1DAY;

    seconds += ( ( uint32_t ) time->Seconds + ( ( uint32_t ) time->Minutes * SECONDS_IN_1MINUTE ) +
                 ( ( uint32_t ) time->Hours * SECONDS_IN_1HOUR ) );

    timestamp_in_ticks = ( ( ( uint64_t ) seconds ) << N_PREDIV_S ) + ( PREDIV_S - time->SubSeconds );

    return timestamp_in_ticks;
}
