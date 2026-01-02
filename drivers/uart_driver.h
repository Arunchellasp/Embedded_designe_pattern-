/*
 * uart_driver.h - UART Driver
 *
 * Application-facing UART driver.
 * Depends only on HAL abstraction.
 */

#ifndef DRIVERS_UART_DRIVER_H
#define DRIVERS_UART_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "../common/error.h"
#include "../hal/hal_uart.h"

/* UART Driver Initialization */
error_t uart_driver_init(void);
error_t uart_driver_deinit(void);

/* UART Driver API */
error_t uart_driver_open(uart_id_t uart_id, uint32_t baud_rate);
error_t uart_driver_close(uart_id_t uart_id);
error_t uart_driver_write(uart_id_t uart_id, const uint8_t *data, uint16_t length);
error_t uart_driver_read(uart_id_t uart_id, uint8_t *data, uint16_t length);
error_t uart_driver_write_string(uart_id_t uart_id, const char *str);

#endif /* DRIVERS_UART_DRIVER_H */
