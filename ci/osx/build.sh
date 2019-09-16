#!/bin/bash


export asl_package_name="asl/0.1@andry/dev"
export conan_def_generator=" cmake_find_package "

git submodule update --init libmomiji/external/asl

# Package asl
cd libmomiji/external/asl
conan create . andry/dev

cd ../../..

mkdir -p build

cd build
conan install --generator ${conan_def_generator} ${asl_package_name}

export PATH="/usr/local/opt/llvm/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/llvm/lib"
export CPPFLAGS="-I/usr/local/opt/llvm/include"

compiler_path="/usr/local/opt/llvm/bin"

export cmake_args=" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=${compiler_path}/clang -DCMAKE_CXX_COMPILER=${compiler_path}/clang++"
export build_command="cmake --build ."

./../ci/osx/build-${1:-lib}.sh
