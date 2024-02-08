/*!
 * @file      spi.h
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

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
    LR1110_SPI_STATUS_OK    = 0,  //!< SPI operation executed successfuly
    LR1110_SPI_STATUS_ERROR = 3,  //!< SPI operation failed
} lr1110_spi_status_t;

/*!
 * @brief GPIO pin state
 */
typedef enum {
    LR1110_SPI_GPIO_PIN_STATE_LOW,
    LR1110_SPI_GPIO_PIN_STATE_HIGH,
} lr1110_spi_gpio_pin_state_t;

/*
 * ============================================================================
 * API definitions to be implemented by the user
 * ============================================================================
 */


/*!
 * @brief Radio data transfer - read
 *
 * @remark Must be implemented by the upper layer
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [out] data            Pointer to the buffer to be received
 * @param [in] data_length      Buffer size to be received
 *
 * @returns Operation status
 */
lr1110_spi_status_t lr1110_spi_read( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length );

void setupSPI();