/*
 * bsp_init.h - Board Support Package: Initialization
 */

#ifndef BSP_INIT_H
#define BSP_INIT_H

#include "../common/error.h"

/* Board Initialization */
error_t bsp_init(void);
error_t bsp_deinit(void);

#endif /* BSP_INIT_H */
