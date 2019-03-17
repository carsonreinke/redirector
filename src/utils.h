#include <stdio.h> //stderr
#include <string.h> //str*

#ifndef UTILS_H
#define UTILS_H
    //Turn of debug unless supplied with `gcc -DDEBUG`
    #ifndef DEBUG
    #define DEBUG 0
    #else
    #define DEBUG 1
    #endif

    #define redirector_debug_print(fmt, ...) \
        do { \
            if (DEBUG) \
                fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__); \
        } while (0)

    unsigned char *redirector_strtolower(const unsigned char* source);

    #define redirector_strlen(source) strlen((const char *)source)

    #define redirector_strstr(haystack, needle) strstr((const char *)(haystack), (const char *)(needle))

    #define redirector_strcpy(dest, src) strcpy((char *)(dest), (const char *)(src))

    #define redirector_strcat(dest, src) strcat((char *)(dest), (const char *)(src))

    #define redirector_strncpy(dest, src, n) strncpy((char *)(dest), (const char *)(src), (n))
#endif
