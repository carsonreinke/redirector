#!/bin/bash

make clean && \
env \
    LD_PRELOAD=/usr/local/lib/libresolv_wrapper.so \
    RESOLV_WRAPPER_HOSTS=$(cd ./tests/; pwd)/resolv-mock.conf \
    RESOLV_WRAPPER_DEBUGLEVEL=4 make check
cat ./tests/test-suite.log