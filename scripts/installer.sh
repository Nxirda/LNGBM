#!/bin/bash

mkdir -p build
cd build 
cmake .. --DCMAKE_BUILD_TYPE=Release

#Need to add a make install thingy in there
