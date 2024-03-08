#include <stdint.h>

#include "helper.h"
#include "main.h"

uint64_t set_bit_x_to_1(const uint8_t position) {
    return 0b00000000000000000000000000000000 | (1 << position);
}

void print_binary(const uint32_t num) {
    for(int i = 7; i >= 0; i--) {
        HAL_DBG_TRACE_PRINTF("%d", (num >> i) & 1);
    }
    HAL_DBG_TRACE_PRINTF(" ");
}

// TODO: Implement these/this functions in the other files

// Function to print the binary representation of an error status
// Inputs:
//   error_status: The error status to be printed in binary
//   length: The length of the bit sequence to be printed
void print_bits(unsigned int error_status, int length) {
    // Iterate through each bit of the error status starting from the most significant bit
    for (int i = length - 1; i >= 0; --i) {
        // Print the current bit by shifting and masking
        HAL_DBG_TRACE_PRINTF("%d", (error_status >> i) & 0x01);
        
        // Insert a space after every 4 bits for better readability, except for the last group of bits
        if ((length - i) % 4 == 0 && i != 0) {
            HAL_DBG_TRACE_PRINTF(" ");
        }
    }
}