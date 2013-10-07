#!/bin/bash
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make Release
make install
