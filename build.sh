#!/bin/bash

cd build
cmake --build .

if [ $? -eq 0 ]; then
    ./momiji
else
    exit 1
fi
