#!/bin/bash

echo "Building from MSYS2"

# Just to be safe
pacman -Syu --noconfirm

# Setup MSYS2 env
pacman -S --noconfirm mingw-w64-x86_64-qt5 mingw-w64-x86_64-cmake p7zip base-devel \
                      mingw-w64-x86_64-clang mingw-w64-x86_64-toolchain

#pip3.exe install conan
#cd ../libmomiji/external/asl
#conan create . andry/dev
#cd ../../../build
#conan install --generator ${conan_def_generator} ${asl_package_name}

cmake.exe .. -G "MSYS Makefiles" -DCMAKE_CXX_COMPILER="clang++" \
                -DMOMIJI_BUILD_QT_GUI=1 -DCMAKE_BUILD_TYPE=Release \
                -DMOMIJI_ENABLE_LTO=0

make -j2
