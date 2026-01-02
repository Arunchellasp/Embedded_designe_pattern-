/*
 * uart_driver.c - UART Driver Implementation
 */

#include "uart_driver.h"
#include <string.h>

error_t uart_driver_init(void)
{
    uart_hal_init();
    return ERR_OK;
}

error_t uart_driver_deinit(void)
{
    return ERR_OK;
}

error_t uart_driver_open(uart_id_t uart_id, uint32_t baud_rate)
{
    uart_config_t config = {
        .uart_id = uart_id,
        .baud_rate = (uart_baud_t)baud_rate,
        .data_bits = UART_DATA_8,
        .stop_bits = UART_STOP_1,
        .parity = UART_PARITY_NONE
    };
    
    return uart_configure(uart_id, &config);
}

error_t uart_driver_close(uart_id_t uart_id)
{
    return uart_deinit(uart_id);
}

error_t uart_driver_write(uart_id_t uart_id, const uint8_t *data, uint16_t length)
{
    if (data == NULL || length == 0) {
        return ERR_INVALID_PARAM;
    }
    return uart_transmit(uart_id, data, length);
}

error_t uart_driver_read(uart_id_t uart_id, uint8_t *data, uint16_t length)
{
    if (data == NULL || length == 0) {
        return ERR_INVALID_PARAM;
    }
    return uart_receive(uart_id, data, length);
}

error_t uart_driver_write_string(uart_id_t uart_id, const char *str)
{
    if (str == NULL) {
        return ERR_INVALID_PARAM;
    }
    uint16_t length = (uint16_t)strlen(str);
    return uart_transmit(uart_id, (const uint8_t *)str, length);
}
