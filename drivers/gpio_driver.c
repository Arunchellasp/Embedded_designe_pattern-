/*
 * gpio_driver.c - GPIO Driver Implementation
 */

#include "gpio_driver.h"

error_t gpio_driver_init(void)
{
    gpio_hal_init();
    return ERR_OK;
}

error_t gpio_driver_deinit(void)
{
    return ERR_OK;
}

error_t gpio_driver_configure(gpio_pin_t pin, gpio_mode_t mode)
{
    gpio_configure(pin, mode, GPIO_OUTPUT_PP, GPIO_PULL_NONE, GPIO_SPEED_HIGH);
    return ERR_OK;
}

error_t gpio_driver_set(gpio_pin_t pin)
{
    gpio_write(pin, true);
    return ERR_OK;
}

error_t gpio_driver_clear(gpio_pin_t pin)
{
    gpio_write(pin, false);
    return ERR_OK;
}

error_t gpio_driver_toggle(gpio_pin_t pin)
{
    gpio_toggle(pin);
    return ERR_OK;
}

error_t gpio_driver_read(gpio_pin_t pin, bool *value)
{
    if (value == NULL) {
        return ERR_INVALID_PARAM;
    }
    *value = gpio_read(pin);
    return ERR_OK;
}
