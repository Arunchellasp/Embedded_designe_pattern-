/*
 * hal_uart.h - UART Hardware Abstraction Layer (HAL)
 *
 * Portable UART interface for different HAL implementations.
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>
#include <stdbool.h>
#include "../common/error.h"

/* UART Peripheral IDs */
typedef enum {
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6
} uart_id_t;

/* UART Baud Rates */
typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_19200 = 19200,
    UART_BAUD_38400 = 38400,
    UART_BAUD_115200 = 115200
} uart_baud_t;

/* UART Data Bits */
typedef enum {
    UART_DATA_8 = 0,
    UART_DATA_9
} uart_data_bits_t;

/* UART Stop Bits */
typedef enum {
    UART_STOP_1 = 0,
    UART_STOP_2
} uart_stop_bits_t;

/* UART Parity */
typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} uart_parity_t;

/* UART Configuration */
typedef struct {
    uart_id_t uart_id;
    uart_baud_t baud_rate;
    uart_data_bits_t data_bits;
    uart_stop_bits_t stop_bits;
    uart_parity_t parity;
} uart_config_t;

/* UART HAL Function Pointers */
typedef struct {
    error_t (*init)(uart_id_t uart_id, const uart_config_t *config);
    error_t (*deinit)(uart_id_t uart_id);
    error_t (*transmit)(uart_id_t uart_id, const uint8_t *data, uint16_t length);
    error_t (*receive)(uart_id_t uart_id, uint8_t *data, uint16_t length);
    error_t (*transmit_it)(uart_id_t uart_id, const uint8_t *data, uint16_t length);
    error_t (*receive_it)(uart_id_t uart_id, uint8_t *data, uint16_t length);
    bool (*is_tx_complete)(uart_id_t uart_id);
    bool (*is_rx_available)(uart_id_t uart_id);
} uart_hal_t;

/* UART HAL API */
void uart_hal_init(void);
error_t uart_configure(uart_id_t uart_id, const uart_config_t *config);
error_t uart_deinit(uart_id_t uart_id);
error_t uart_transmit(uart_id_t uart_id, const uint8_t *data, uint16_t length);
error_t uart_receive(uart_id_t uart_id, uint8_t *data, uint16_t length);
error_t uart_transmit_it(uart_id_t uart_id, const uint8_t *data, uint16_t length);
error_t uart_receive_it(uart_id_t uart_id, uint8_t *data, uint16_t length);
bool uart_is_tx_complete(uart_id_t uart_id);
bool uart_is_rx_available(uart_id_t uart_id);

#endif /* HAL_UART_H */
