#include <stdlib.h> //calloc, NULL
#include <ctype.h> //tolower
#include <string.h> //strlen

unsigned char *redirector_strtolower(const unsigned char* source) {
    unsigned char *dest;
    
    if(source == NULL) {
        return NULL;
    }

    dest = calloc(strlen(source) + 1, sizeof(unsigned char));
    for(int index = 0; index < strlen(source); index++) {
        dest[index] = tolower(source[index]);
    }

    return dest;
}