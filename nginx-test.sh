#!/bin/bash

export MODULE_PATH=$(pwd $0)
export NGINX_PATH=$(cd $1; pwd)

#Augment PATH with Nginx for Test::Nginx
export PATH="$PATH:$NGINX_PATH/objs"

#Make Nginx with our module
cd $NGINX_PATH
env \
    CFLAGS="-g -O0 -DDEBUG" \
    ./configure --with-debug --add-module=$MODULE_PATH >/dev/null && $(which make) >/dev/null

#Run Test::Nginx (only if compile did not fail)
if [ $? -eq 0 ]; then
    cd $MODULE_PATH
    env \
        LD_PRELOAD=/usr/local/lib/libresolv_wrapper.so \
        RESOLV_WRAPPER_HOSTS=$MODULE_PATH/tests/resolv-mock.conf \
        RESOLV_WRAPPER_DEBUGLEVEL=4 \
        $(which prove) -v -r $MODULE_PATH/nginx-tests/
fi