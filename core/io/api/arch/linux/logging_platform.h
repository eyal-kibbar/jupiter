#ifndef LOGGING_PLATFORM_H_
#define LOGGING_PLATFORM_H_


#include <stdio.h>


#define PLATFORM_LOG_PRINT(prefix, module, fmt, ...) \
    printf(prefix module " # " fmt "\n", ##__VA_ARGS__)

#define PLATFORM_LOG_ASSERT(condition, fmt, ...)            \
    do {                                                    \
        if (!(condition)) {                                 \
            LOG_PRINT("ASSERT:", __FILE__,                  \
                  "line %d: " fmt, __LINE__, ##__VA_ARGS__);\
            while (1);                                      \
        }                                                   \
    } while (0)

#endif /* LOGGING_PLATFORM_H_ */
