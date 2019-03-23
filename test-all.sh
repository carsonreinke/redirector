#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo 'Usage: ./test-all.sh [RESOLV WRAPPER FILE] [NGINX PATH]'
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo 'RESOLV WRAPPER FILE must point to an existing file'
    exit 1
fi

if [[ ! -d $2 ]]; then
    echo 'NGINX PATH must point to a Nginx source directory'
    exit 1
fi

export RESOLV_WRAPPER_FILE="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"
export NGINX_PATH=$(cd $2; pwd)

./test.sh $RESOLV_WRAPPER_FILE
if [[ $? -ne 0 ]]; then
    exit 1
fi

./test-valgrind.sh $RESOLV_WRAPPER_FILE
if [[ $? -ne 0 ]]; then
    exit 1
fi

./test-nginx.sh $RESOLV_WRAPPER_FILE $NGINX_PATH
if [[ $? -ne 0 ]]; then
    exit 1
fi