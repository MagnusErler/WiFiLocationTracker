#include <stdint.h>

#include "helper.h"

uint64_t set_bit_x_to_1(uint64_t byte, uint8_t position) {
    return byte | (1 << position);
}