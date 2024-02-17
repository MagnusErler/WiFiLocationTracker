

#include "smtc_hal_gpio_pin_names.h"


/*!
 * @brief Prints debug trace
 *
 * @param variadics arguments
 */
void hal_mcu_trace_print( const char* fmt, ... );

/*!
 * @brief Initializes UART
 *
 * @param id UART id
 * @param uart_tx UART TX pin
 * @param uart_rx UART RX pin
 */
void hal_uart_init( const uint32_t id, const hal_gpio_pin_names_t uart_tx, const hal_gpio_pin_names_t uart_rx );