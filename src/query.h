#ifndef QUERY_H
#define QUERY_H
    #define REDIRECTOR_ERROR -2
    #define REDIRECTOR_OK -1
    #define REDIRECTOR_UNSUCCESSFUL(result) (result == REDIRECTOR_ERROR || result == REDIRECTOR_OK)
    #define REDIRECTOR_SUCCESSFUL(result) (!REDIRECTOR_UNSUCCESSFUL(result))

    extern void redirector_query_deinit();
    extern int redirector_query_txt(const unsigned char *domain, unsigned char **dest);
    extern int redirector_query_uri(const unsigned char *domain, unsigned char **dest);
#endif