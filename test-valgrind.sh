#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo 'Usage: ./test-valgrind.sh [RESOLV WRAPPER FILE]'
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo 'RESOLV WRAPPER FILE must point to an existing file'
    exit 1
fi

export MODULE_PATH=$(pwd $0)
export RESOLV_WRAPPER_FILE="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"

$MODULE_PATH/test.sh $RESOLV_WRAPPER_FILE && \
env \
    CK_FORK=no \
    LD_PRELOAD=$RESOLV_WRAPPER_FILE \
    RESOLV_WRAPPER_HOSTS=$MODULE_PATH/tests/resolv-mock.conf \
    RESOLV_WRAPPER_DEBUGLEVEL=0 \
    libtool --mode=execute valgrind --leak-check=full $MODULE_PATH/tests/check_redirector