#!/bin/bash

./test.sh && \
env \
    CK_FORK=no \
    LD_PRELOAD=/usr/local/lib/libresolv_wrapper.so \
    RESOLV_WRAPPER_HOSTS=$(cd ./tests/; pwd)/resolv-mock.conf \
    RESOLV_WRAPPER_DEBUGLEVEL=0 \
    libtool --mode=execute valgrind --leak-check=full ./tests/check_redirector