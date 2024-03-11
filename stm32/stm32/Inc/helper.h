#include <stdint.h>

/*!
 * @brief Set a bit to 1 at a given position
 *
 * @param position The position of the bit to set to 1
 * 
 * @return The modified byte
 */
uint64_t set_bit_x_to_1(const uint8_t position);

void print_bits(unsigned int error_status, int length);

/*!
 * @brief Print a number in binary format
 *
 * @param num The number to print in binary format
 */
void print_binary(const uint32_t num);