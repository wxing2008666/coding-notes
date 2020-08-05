#!/usr/bin/env bash

set -x
# Cause the script to exit if a single command fails.
set -e

cd cpp_web_demo
cmake .
make

