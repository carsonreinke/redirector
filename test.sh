#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo 'Usage: ./test.sh [RESOLV WRAPPER FILE]'
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo 'RESOLV WRAPPER FILE must point to an existing file'
    exit 1
fi

export MODULE_PATH=$(pwd $0)
export RESOLV_WRAPPER_FILE="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"

make clean >/dev/null && \
env \
    LD_PRELOAD=$RESOLV_WRAPPER_FILE \
    RESOLV_WRAPPER_HOSTS=$MODULE_PATH/tests/resolv-mock.conf \
    RESOLV_WRAPPER_DEBUGLEVEL=4 make check
cat $MODULE_PATH/tests/test-suite.log