#include <stdint.h>

#include "helper.h"

uint64_t set_bit_x_to_1(uint8_t position) {
    return 0b00000000000000000000000000000000 | (1 << position);
}