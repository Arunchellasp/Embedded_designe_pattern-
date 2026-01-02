/*
 * error.c - Error Handling Implementation
 */

#include "error.h"

#define ERROR_LOG_SIZE 32

typedef struct {
    error_entry_t log[ERROR_LOG_SIZE];
    uint32_t count;
    uint32_t last_index;
} error_manager_t;

static error_manager_t error_mgr = {0};

void error_init(void)
{
    error_mgr.count = 0;
    error_mgr.last_index = 0;
}

void error_log(error_t error_code, error_severity_t severity, uint32_t context)
{
    uint32_t idx = error_mgr.last_index % ERROR_LOG_SIZE;
    
    error_mgr.log[idx].error_code = error_code;
    error_mgr.log[idx].severity = severity;
    error_mgr.log[idx].context = context;
    error_mgr.log[idx].timestamp = 0;  /* TODO: Use system tick */
    
    error_mgr.last_index++;
    if (error_mgr.count < ERROR_LOG_SIZE) {
        error_mgr.count++;
    }
}

error_t error_get_last(void)
{
    if (error_mgr.count == 0) {
        return ERR_OK;
    }
    uint32_t idx = (error_mgr.last_index - 1) % ERROR_LOG_SIZE;
    return error_mgr.log[idx].error_code;
}

error_severity_t error_get_last_severity(void)
{
    if (error_mgr.count == 0) {
        return SEVERITY_INFO;
    }
    uint32_t idx = (error_mgr.last_index - 1) % ERROR_LOG_SIZE;
    return error_mgr.log[idx].severity;
}

uint32_t error_get_count(void)
{
    return error_mgr.count;
}

void error_clear_last(void)
{
    if (error_mgr.count > 0) {
        error_mgr.count--;
    }
}
