/*
 * hal_uart.c - UART HAL Implementation (Stub - Ready for STM32 HAL integration)
 */

#include "hal_uart.h"
#include "../bsp/board_config.h"

static uart_hal_t *uart_hal = NULL;

/* ===== STM32 HAL Stub Functions ===== */

static error_t stm32_uart_init(uart_id_t uart_id, const uart_config_t *config)
{
    /* TODO: Implement STM32 HAL UART initialization
     * 1. Configure GPIO pins (TX, RX)
     * 2. Setup UART handle
     * 3. Initialize with config parameters
     */
    (void)uart_id; (void)config;
    return ERR_OK;
}

static error_t stm32_uart_deinit(uart_id_t uart_id)
{
    /* TODO: HAL_UART_DeInit() */
    (void)uart_id;
    return ERR_OK;
}

static error_t stm32_uart_transmit(uart_id_t uart_id, const uint8_t *data, uint16_t length)
{
    /* TODO: HAL_UART_Transmit() */
    (void)uart_id; (void)data; (void)length;
    return ERR_OK;
}

static error_t stm32_uart_receive(uart_id_t uart_id, uint8_t *data, uint16_t length)
{
    /* TODO: HAL_UART_Receive() */
    (void)uart_id; (void)data; (void)length;
    return ERR_OK;
}

static error_t stm32_uart_transmit_it(uart_id_t uart_id, const uint8_t *data, uint16_t length)
{
    /* TODO: HAL_UART_Transmit_IT() */
    (void)uart_id; (void)data; (void)length;
    return ERR_OK;
}

static error_t stm32_uart_receive_it(uart_id_t uart_id, uint8_t *data, uint16_t length)
{
    /* TODO: HAL_UART_Receive_IT() */
    (void)uart_id; (void)data; (void)length;
    return ERR_OK;
}

static bool stm32_uart_is_tx_complete(uart_id_t uart_id)
{
    /* TODO: Check UART status */
    (void)uart_id;
    return true;
}

static bool stm32_uart_is_rx_available(uart_id_t uart_id)
{
    /* TODO: Check if data is available */
    (void)uart_id;
    return false;
}

static const uart_hal_t stm32_uart_hal = {
    .init = stm32_uart_init,
    .deinit = stm32_uart_deinit,
    .transmit = stm32_uart_transmit,
    .receive = stm32_uart_receive,
    .transmit_it = stm32_uart_transmit_it,
    .receive_it = stm32_uart_receive_it,
    .is_tx_complete = stm32_uart_is_tx_complete,
    .is_rx_available = stm32_uart_is_rx_available
};

/* ===== HAL Abstraction API ===== */

void uart_hal_init(void)
{
#ifdef USE_STM32_HAL
    uart_hal = (uart_hal_t *)&stm32_uart_hal;
#elif defined(USE_STM32_LL)
    /* uart_hal = &stm32_ll_uart_hal; */
#elif defined(USE_OPENCM3)
    /* uart_hal = &opencm3_uart_hal; */
#else
    uart_hal = (uart_hal_t *)&stm32_uart_hal;
#endif
}

error_t uart_configure(uart_id_t uart_id, const uart_config_t *config)
{
    if (uart_hal == NULL || uart_hal->init == NULL) {
        return ERR_NOT_INITIALIZED;
    }
    return uart_hal->init(uart_id, config);
}

error_t uart_deinit(uart_id_t uart_id)
{
    if (uart_hal == NULL || uart_hal->deinit == NULL) {
        return ERR_NOT_INITIALIZED;
    }
    return uart_hal->deinit(uart_id);
}

error_t uart_transmit(uart_id_t uart_id, const uint8_t *data, uint16_t length)
{
    if (uart_hal == NULL || uart_hal->transmit == NULL) {
        return ERR_NOT_INITIALIZED;
    }
    return uart_hal->transmit(uart_id, data, length);
}

error_t uart_receive(uart_id_t uart_id, uint8_t *data, uint16_t length)
{
    if (uart_hal == NULL || uart_hal->receive == NULL) {
        return ERR_NOT_INITIALIZED;
    }
    return uart_hal->receive(uart_id, data, length);
}

error_t uart_transmit_it(uart_id_t uart_id, const uint8_t *data, uint16_t length)
{
    if (uart_hal == NULL || uart_hal->transmit_it == NULL) {
        return ERR_NOT_INITIALIZED;
    }
    return uart_hal->transmit_it(uart_id, data, length);
}

error_t uart_receive_it(uart_id_t uart_id, uint8_t *data, uint16_t length)
{
    if (uart_hal == NULL || uart_hal->receive_it == NULL) {
        return ERR_NOT_INITIALIZED;
    }
    return uart_hal->receive_it(uart_id, data, length);
}

bool uart_is_tx_complete(uart_id_t uart_id)
{
    if (uart_hal == NULL || uart_hal->is_tx_complete == NULL) {
        return false;
    }
    return uart_hal->is_tx_complete(uart_id);
}

bool uart_is_rx_available(uart_id_t uart_id)
{
    if (uart_hal == NULL || uart_hal->is_rx_available == NULL) {
        return false;
    }
    return uart_hal->is_rx_available(uart_id);
}
