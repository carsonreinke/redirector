#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo 'Usage: ./test-nginx.sh [RESOLV WRAPPER FILE] [NGINX PATH]'
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

export MODULE_PATH=$(pwd $0)
export RESOLV_WRAPPER_FILE="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"
export NGINX_PATH=$(cd $2; pwd)

#Augment PATH with Nginx for Test::Nginx
export PATH="$PATH:$NGINX_PATH/objs"

#Make Nginx with our module
cd $NGINX_PATH
env \
    CFLAGS="-g -O0 -DDEBUG" \
    ./configure --with-debug --add-module=$MODULE_PATH >/dev/null && $(which make) >/dev/null

#Run Test::Nginx (only if compile did not fail)
if [[ $? -ne 0 ]]; then
    exit $?
fi

cd $MODULE_PATH
env \
    LD_PRELOAD=$RESOLV_WRAPPER_FILE \
    $(which prove) -v -r $MODULE_PATH/nginx-tests/