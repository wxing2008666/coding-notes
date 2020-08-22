#!/usr/bin/env bash

set -x
# Cause the script to exit if a single command fails.
set -e

if which clang-format >/dev/null; then
    CLANG_FORMAT_VERSION=$(clang-format --version | awk '{print $3}')
    echo $CLANG_FORMAT_VERSION
else
    echo "WARNING: clang-format is not installed!"
    #sudo apt-get install clang-format
fi

cd hello_world
g++ -o main hello_world.cpp

cd ..

#msgpack require boost
#cd msgpack_test
#g++ -o test test.cpp -I./include

#cd ..

cd cpp_web_demo
cmake .
make

