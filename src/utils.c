#include <stdlib.h> //calloc, NULL
#include <ctype.h> //tolower
#include <string.h> //strlen
#include "utils.h"

unsigned char *redirector_strtolower(const unsigned char* source) {
    unsigned char *dest;
    
    if(source == NULL) {
        return NULL;
    }

    dest = calloc(redirector_strlen(source) + 1, sizeof(unsigned char));
    if(dest == NULL) {
        redirector_debug_print("%s", "calloc failed");
        return NULL;
    }
    for(size_t index = 0; index < redirector_strlen(source); index++) {
        dest[index] = tolower(source[index]);
    }

    return dest;
}