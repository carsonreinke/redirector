#include <string.h>
#include <resolv.h>

#ifndef QUERY_H
#define QUERY_H
    size_t redirector_query_txt(const unsigned char *domain, unsigned char **dest);
#endif