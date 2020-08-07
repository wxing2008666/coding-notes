#!/usr/bin/env bash

set -x
# Cause the script to exit if a single command fails.
set -e

cd hello_world
g++ -o main hello_world.cpp

cd ..

cd msgpack_test
g++ -o test test.cpp -I./include

cd ..

cd cpp_web_demo
cmake .
make

