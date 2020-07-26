#ifndef LOGGING_H_
#define LOGGING_H_

#include "logging_platform.h"


#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#define LOG_ASSERT(condition, fmt, ...) \
    PLATFORM_LOG_ASSERT(condition, fmt, ##__VA_ARGS__)


#if LOG_LEVEL >= 3
#define LOG_INFO(module, fmt, ...) \
    PLATFORM_LOG_PRINT("INFO: ", #module, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(module, fmt, ...)
#endif


#if LOG_LEVEL >= 2
#define LOG_WARN(module, fmt, ...) \
    PLATFORM_LOG_PRINT("WARNING: ", #module, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(module, fmt, ...)
#endif


#if LOG_LEVEL >= 1
#define LOG_ERROR(module, fmt, ...) \
    PLATFORM_LOG_PRINT("ERROR: ", #module, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(module, fmt, ...)
#endif

#endif /*  LOGGING_H_ */
