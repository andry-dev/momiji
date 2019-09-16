#!/bin/bash

export asl_package_name="asl/0.1@andry/dev"
export conan_def_generator=" cmake_find_package "

git submodule update --init libmomiji/external/asl

cd libmomiji/external/asl
conan create . andry/dev

cd ../../..

mkdir -p build

cd build
conan install --generator ${conan_def_generator} ${asl_package_name}

export cmake_args=" -A x64 -DCMAKE_BUILD_TYPE=Release "
export build_command="cmake --build . --config Release"

./../ci/win-x64/build-${1:-lib}.sh
