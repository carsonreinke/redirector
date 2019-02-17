#include <stdio.h> //stderr

#ifndef UTILS_H
#define UTILS_H
    //Turn of debug unless supplied with `gcc -DDEBUG`
    #ifndef DEBUG
    #define DEBUG 0
    #else
    #define DEBUG 1
    #endif
    
    #define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, \
                                    __LINE__, __func__, __VA_ARGS__); } while (0)
    
    unsigned char *redirector_strtolower(const unsigned char* source);
#endif
