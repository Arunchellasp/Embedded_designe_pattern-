/*
 * app.h - Application Layer
 *
 * Main application logic and state machine.
 * No HAL or driver details here - only business logic.
 */

#ifndef APP_APP_H
#define APP_APP_H

#include "../common/error.h"

/* Application States */
typedef enum {
    APP_STATE_INIT = 0,
    APP_STATE_RUNNING,
    APP_STATE_ERROR,
    APP_STATE_SHUTDOWN
} app_state_t;

/* Application Initialization */
error_t app_init(void);
error_t app_start(void);
error_t app_stop(void);

/* Main Application Loop */
error_t app_run(void);

/* Health Check */
error_t app_health_check(void);
app_state_t app_get_state(void);

#endif /* APP_APP_H */
