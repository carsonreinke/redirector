#!/bin/bash

export MODULE_PATH=$(pwd $0)

$MODULE_PATH/test.sh && \
env \
    CK_FORK=no \
    LD_PRELOAD=/usr/local/lib/libresolv_wrapper.so \
    RESOLV_WRAPPER_HOSTS=$MODULE_PATH/tests/resolv-mock.conf \
    RESOLV_WRAPPER_DEBUGLEVEL=0 \
    libtool --mode=execute valgrind --leak-check=full $MODULE_PATH/tests/check_redirector