#include <stdint.h>
#include <stdbool.h>

#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_spi.h"

/*!
 * @brief Status code of SPI operations
 */
typedef enum lr1110_spi_status_e {
    LR1110_SPI_STATUS_OK        = 0x00,  //!< SPI operation executed successfuly
    LR1110_SPI_STATUS_TIMEOUT   = 0x01,  //!< Timeout occured while waiting for a line state
    LR1110_SPI_STATUS_ERROR     = 0xFF,  //!< SPI operation failed
} lr1110_spi_status_t;

/*!
 * @brief Radio data transfer - write
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] cbuffer          Pointer to the buffer to be transmitted
 * @param [in] cbuffer_length   Buffer size to be transmitted
 * @param [in] get_status       Get status after the transfer
 * 
 * @returns Operation status
 */
lr1110_spi_status_t lr1110_spi_write(const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length);

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
lr1110_spi_status_t lr1110_spi_read(const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length);

/**
 * @brief Prints interrupt and command status information received from the LR1110 radio chip.
 *
 * @param [in] stat1    Status byte received from the LR1110 radio chip.
 */
void printStat1(uint8_t stat1);

/**
 * @brief Prints if chip is in bootloader execution, chip mode and reset status information received from the LR1110 radio chip. 
 *
 * @param [in] stat2    Status byte received from the LR1110 radio chip.
 */
void printStat2(uint8_t stat2);

/**
 * @brief Prints the IRQ buffer received from the LR1110 radio chip.
 *
 * @param [in] buffer          Pointer to the buffer to be received
 * @param [in] buffer_length   Buffer size to be received
 */
void printIrq(const uint8_t* buffer, const uint16_t buffer_length);