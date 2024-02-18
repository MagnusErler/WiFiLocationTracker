/*!
 * @file      spi.h
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_spi.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/**
 * @brief Write this to SPI bus while reading data, or as a dummy/placeholder
 */
#define LR1110_NOP ( 0x00 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief Status code of HAL operation
 */
typedef enum lr1110_spi_status_e {
    LR1110_SPI_STATUS_OK        = 0x00,  //!< SPI operation executed successfuly
    LR1110_SPI_STATUS_TIMEOUT   = 0x01,  //!< Timeout occured while waiting for a line state
    LR1110_SPI_STATUS_ERROR     = 0xFF,  //!< SPI operation failed
} lr1110_spi_status_t;

/*
 * ============================================================================
 * API definitions to be implemented by the user
 * ============================================================================
 */

/*!
 * @brief Wait for a GPIO pin to reach a specific state
 *
 * @param [in] context      Radio implementation parameters
 * @param [in] cbuffer      Pointer to the buffer to be transmitted
 * @param [in] cbuffer_length   Buffer size to be transmitted
 * 
 * @returns Operation status
 */
lr1110_spi_status_t lr1110_spi_write( const void* context, const uint8_t* cbuffer, uint16_t cbuffer_length );

/*!
 * @brief Radio data transfer - read
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] cbuffer          Pointer to the buffer to be transmitted
 * @param [in] cbuffer_length   Buffer size to be transmitted
 * @param [out] rbuffer         Pointer to the buffer to be received
 * @param [in] rbuffer_length   Buffer size to be received
 *
 * @returns Operation status
 */
lr1110_spi_status_t lr1110_spi_read( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length );