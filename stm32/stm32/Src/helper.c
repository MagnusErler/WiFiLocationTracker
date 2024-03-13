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