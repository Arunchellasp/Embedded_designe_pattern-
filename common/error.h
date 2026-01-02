/*
 * error.h - Centralized Error Handling
 * 
 * Provides industrial-grade error codes, severity levels, and error management.
 * No silent failures allowed.
 */

#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H

#include <stdint.h>
#include <stdbool.h>

/* Error Codes */
typedef enum {
    ERR_OK              = 0x00,  /* No error */
    ERR_TIMEOUT         = 0x01,  /* Operation timeout */
    ERR_HW_FAILURE      = 0x02,  /* Hardware failure detected */
    ERR_INVALID_PARAM   = 0x03,  /* Invalid parameter */
    ERR_NOT_INITIALIZED = 0x04,  /* Module not initialized */
    ERR_BUSY            = 0x05,  /* Device busy */
    ERR_MEMORY          = 0x06,  /* Memory allocation failed */
    ERR_UNKNOWN         = 0xFF   /* Unknown error */
} error_t;

/* Error Severity Levels */
typedef enum {
    SEVERITY_INFO       = 0,     /* Informational message */
    SEVERITY_WARN       = 1,     /* Warning condition */
    SEVERITY_ERROR      = 2,     /* Error condition */
    SEVERITY_FATAL      = 3      /* Fatal error - system may need reset */
} error_severity_t;

/* Error Log Entry */
typedef struct {
    error_t error_code;
    error_severity_t severity;
    uint32_t timestamp;
    uint32_t context;            /* Additional context information */
} error_entry_t;

/* Error Manager */
void error_init(void);
void error_log(error_t error_code, error_severity_t severity, uint32_t context);
error_t error_get_last(void);
error_severity_t error_get_last_severity(void);
uint32_t error_get_count(void);
void error_clear_last(void);

/* Inline error checking macro */
#define ERROR_CHECK(expr, error_code) do { \
    if (!(expr)) { \
        error_log(error_code, SEVERITY_ERROR, __LINE__); \
        return error_code; \
    } \
} while(0)

#endif /* COMMON_ERROR_H */
