#!/bin/bash

make clean
cmake src -DCMAKE_BUILD_TYPE=Release
make

