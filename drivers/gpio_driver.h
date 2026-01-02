/*
 * gpio_driver.h - GPIO Driver
 *
 * Application-facing GPIO driver.
 * Depends only on HAL abstraction, not on specific hardware.
 */

#ifndef DRIVERS_GPIO_DRIVER_H
#define DRIVERS_GPIO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "../common/error.h"
#include "../hal/hal_gpio.h"

/* GPIO Driver Initialization */
error_t gpio_driver_init(void);
error_t gpio_driver_deinit(void);

/* GPIO Driver API */
error_t gpio_driver_configure(gpio_pin_t pin, gpio_mode_t mode);
error_t gpio_driver_set(gpio_pin_t pin);
error_t gpio_driver_clear(gpio_pin_t pin);
error_t gpio_driver_toggle(gpio_pin_t pin);
error_t gpio_driver_read(gpio_pin_t pin, bool *value);

#endif /* DRIVERS_GPIO_DRIVER_H */
