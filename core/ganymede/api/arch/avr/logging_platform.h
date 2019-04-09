#ifndef LOGGING_PLATFORM_H_
#define LOGGING_PLATFORM_H_


#include <avr/pgmspace.h>
#include <stdio.h>


#define PLATFORM_LOG_PRINT(prefix, module, fmt, ...)   \
    do {                                        \
        printf_P(PSTR(prefix module " # "));    \
        printf_P(PSTR(fmt), ##__VA_ARGS__);     \
        printf_P(PSTR("\n\r"));                 \
    } while(0)


#define PLATFORM_LOG_ASSERT(condition, fmt, ...)            \
    do {                                                    \
        if (!(condition)) {                                 \
            LOG_PRINT("ASSERT:", __FILE__,                  \
                  "line %d: " fmt, __LINE__, ##__VA_ARGS__);\
            while (1);                                      \
        }                                                   \
    } while (0)

#endif /* LOGGING_PLATFORM_H_ */
