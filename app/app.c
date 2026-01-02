/*
 * app.c - Application Implementation
 *
 * Industrial-style application:
 * - Proper initialization sequence
 * - Error handling
 * - Health checks
 * - LED heartbeat
 */

#include "app.h"
#include "../bsp/bsp_init.h"
#include "../bsp/board_config.h"
#include "../drivers/gpio_driver.h"
#include "../drivers/uart_driver.h"
#include "../common/error.h"

static app_state_t app_state = APP_STATE_INIT;
static uint32_t heartbeat_counter = 0;

error_t app_init(void)
{
    error_t err;

    /* Initialize error system first */
    error_init();

    /* Initialize BSP */
    err = bsp_init();
    if (err != ERR_OK) {
        error_log(err, SEVERITY_FATAL, 0);
        app_state = APP_STATE_ERROR;
        return err;
    }

    /* Initialize GPIO driver */
    err = gpio_driver_init();
    if (err != ERR_OK) {
        error_log(err, SEVERITY_FATAL, 1);
        app_state = APP_STATE_ERROR;
        return err;
    }

    /* Initialize UART driver */
    err = uart_driver_init();
    if (err != ERR_OK) {
        error_log(err, SEVERITY_FATAL, 2);
        app_state = APP_STATE_ERROR;
        return err;
    }

    app_state = APP_STATE_RUNNING;
    return ERR_OK;
}

error_t app_start(void)
{
    if (app_state != APP_STATE_RUNNING) {
        return ERR_NOT_INITIALIZED;
    }
    return ERR_OK;
}

error_t app_stop(void)
{
    app_state = APP_STATE_SHUTDOWN;
    return ERR_OK;
}

error_t app_run(void)
{
    if (app_state != APP_STATE_RUNNING) {
        return ERR_NOT_INITIALIZED;
    }

    heartbeat_counter++;

    /* Simple heartbeat: toggle LED every 1000 iterations */
    if (heartbeat_counter % 1000 == 0) {
        gpio_driver_toggle((gpio_pin_t)LED_PIN);
    }

    /* Health check */
    error_t err = app_health_check();
    if (err != ERR_OK) {
        app_state = APP_STATE_ERROR;
        return err;
    }

    return ERR_OK;
}

error_t app_health_check(void)
{
    error_t last_err = error_get_last();
    
    if (last_err != ERR_OK) {
        error_severity_t severity = error_get_last_severity();
        
        if (severity == SEVERITY_FATAL) {
            app_state = APP_STATE_ERROR;
            return last_err;
        }
    }

    return ERR_OK;
}

app_state_t app_get_state(void)
{
    return app_state;
}
