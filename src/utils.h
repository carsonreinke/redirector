#define DEBUG 1
#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
