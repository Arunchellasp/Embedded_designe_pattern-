/*
 * hal_gpio.h - GPIO Hardware Abstraction Layer (HAL)
 *
 * Portable GPIO interface supporting multiple HAL implementations:
 * - STM32 HAL
 * - STM32 LL
 * - libopencm3
 * - Custom HAL
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/* GPIO Pin Definition */
typedef uint32_t gpio_pin_t;

/* GPIO Mode */
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG
} gpio_mode_t;

/* GPIO Output Type */
typedef enum {
    GPIO_OUTPUT_PP = 0,  /* Push-Pull */
    GPIO_OUTPUT_OD       /* Open-Drain */
} gpio_output_type_t;

/* GPIO Pull Configuration */
typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} gpio_pull_t;

/* GPIO Speed */
typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH
} gpio_speed_t;

/* GPIO HAL Function Pointers */
typedef struct {
    void (*init)(gpio_pin_t pin, gpio_mode_t mode, gpio_output_type_t otype, 
                 gpio_pull_t pull, gpio_speed_t speed);
    void (*write)(gpio_pin_t pin, bool value);
    bool (*read)(gpio_pin_t pin);
    void (*toggle)(gpio_pin_t pin);
} gpio_hal_t;

/* GPIO HAL API */
void gpio_hal_init(void);
void gpio_configure(gpio_pin_t pin, gpio_mode_t mode, gpio_output_type_t otype,
                   gpio_pull_t pull, gpio_speed_t speed);
void gpio_write(gpio_pin_t pin, bool value);
bool gpio_read(gpio_pin_t pin);
void gpio_toggle(gpio_pin_t pin);

#endif /* HAL_GPIO_H */
