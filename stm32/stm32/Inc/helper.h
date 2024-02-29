#include <stdint.h>

/*!
 * @brief Set a bit to 1 at a given position
 *
 * @param byte The byte to modify
 * @param position The position of the bit to set to 1
 * 
 * @return The modified byte
 */
uint64_t set_bit_x_to_1(uint64_t byte, uint8_t position);