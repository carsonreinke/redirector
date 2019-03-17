#!/bin/bash

export NGINX_PATH=$(cd $1; pwd)

./test.sh
./test-valgrind.sh
./test-nginx.sh $NGINX_PATH