#!/bin/bash

cd build

export CPACK_GENERATOR="NSIS;7Z"

cmake --build . --target package --config Release
