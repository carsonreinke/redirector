#!/bin/bash

export MODULE_PATH=$(pwd $0)

make clean >/dev/null && \
env \
    LD_PRELOAD=/usr/local/lib/libresolv_wrapper.so \
    RESOLV_WRAPPER_HOSTS=$MODULE_PATH/tests/resolv-mock.conf \
    RESOLV_WRAPPER_DEBUGLEVEL=4 make check
cat $MODULE_PATH/tests/test-suite.log