/*
 * bsp_clock.h - Board Support Package: Clock Configuration
 */

#ifndef BSP_CLOCK_H
#define BSP_CLOCK_H

#include <stdint.h>
#include "../common/error.h"

/* Clock Configuration Structure */
typedef struct {
    uint32_t system_clock_hz;
    uint32_t ahb_clock_hz;
    uint32_t apb1_clock_hz;
    uint32_t apb2_clock_hz;
} clock_config_t;

/* Clock Initialization */
error_t bsp_clock_init(void);
error_t bsp_clock_get_config(clock_config_t *config);
uint32_t bsp_clock_get_system_clock(void);
uint32_t bsp_clock_get_ahb_clock(void);
uint32_t bsp_clock_get_apb1_clock(void);
uint32_t bsp_clock_get_apb2_clock(void);

#endif /* BSP_CLOCK_H */
