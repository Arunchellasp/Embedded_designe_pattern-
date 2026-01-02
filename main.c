/*
 * main.c - Main Entry Point
 *
 * This is the application entry point after platform startup.
 * All business logic is isolated in the app layer.
 */

#include "app/app.h"
#include "common/error.h"

/* Busy-wait delay (in main loop iterations) */
static void delay_ms(uint32_t ms)
{
    /* TODO: Replace with actual timer-based delay */
    volatile uint32_t counter = ms * 1000;
    while (counter--);
}

int main(void)
{
    error_t err;

    /* Initialize application */
    err = app_init();
    if (err != ERR_OK) {
        /* Fatal error during init - system should not proceed */
        while (1) {
            delay_ms(1);
        }
    }

    /* Start application */
    err = app_start();
    if (err != ERR_OK) {
        app_stop();
        return (int)err;
    }

    /* Main application loop */
    while (app_get_state() == APP_STATE_RUNNING) {
        err = app_run();
        if (err != ERR_OK) {
            /* Log error but continue running */
            error_log(err, SEVERITY_WARN, 0);
        }
        delay_ms(1);
    }

    /* Shutdown */
    app_stop();

    /* System halted */
    while (1);

    return 0;
}
