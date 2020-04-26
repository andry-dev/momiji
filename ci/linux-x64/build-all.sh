#!/bin/bash


# Package tewi
git submodule update --init --recursive ../momiji-gl/external/tewi
cd ../momiji-gl/external/tewi/
conan create . andry/dev

cd ../../../build

conan install --build=missing ..


cmake .. ${cmake_args} \
    -DMOMIJI_BUILD_TOOLS=1 \
    -DMOMIJI_BUILD_GL_GUI=1 \
    -DMOMIJI_BUILD_QT_GUI=1

${build_command}
