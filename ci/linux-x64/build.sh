#!/bin/bash

export asl_package_name="asl/0.1@andry/dev"
export conan_def_generator=" cmake_find_package "

# This scripts expects sources in the /src dir
cd /src

git submodule update --init libmomiji/external/asl

# Package asl
cd libmomiji/external/asl
conan create . andry/dev

cd ../../..

mkdir -p build

cd build
conan install --generator ${conan_def_generator} ${asl_package_name}

export cmake_args=" -G Ninja -DCMAKE_BUILD_TYPE=Release "
export build_command="ninja"

./../ci/linux-x64/build-${1:-lib}.sh
