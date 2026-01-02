/*
 * platform_startup.h - Platform-specific startup
 */

#ifndef PLATFORM_STARTUP_H
#define PLATFORM_STARTUP_H

#include "../common/error.h"

/* Platform initialization (before main()) */
error_t platform_init(void);

#endif /* PLATFORM_STARTUP_H */
