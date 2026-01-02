/*
 * bsp_init.c - Board Support Package: Initialization Implementation
 */

#include "bsp_init.h"
#include "bsp_clock.h"
#include "board_config.h"
#include "../common/error.h"

error_t bsp_init(void)
{
    error_t err;

    /* Initialize clock system */
    err = bsp_clock_init();
    if (err != ERR_OK) {
        error_log(err, SEVERITY_FATAL, 0);
        return err;
    }

    /* TODO: Enable peripheral clocks
     * - GPIO clocks
     * - UART clocks
     * - Timer clocks
     * - etc.
     */

    return ERR_OK;
}

error_t bsp_deinit(void)
{
    /* Cleanup and power-down sequences */
    return ERR_OK;
}
