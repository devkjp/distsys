#!/bin/bash

OS=`uname -s`
ARCH=`uname -m`
BUILD_DIR=$OS"_"$ARCH

./build/$BUILD_DIR/tinyweb -p 8080 -d web
echo "Exit status: " $?

