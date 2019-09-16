#!/bin/bash

git submodule update --init --recursive ../momiji-gl/external/tewi

cd ../momiji-gl/external/tewi
conan create . andry/dev -b glfw -b stb -b glm -b tewi

cd ../../../build

conan install ..

../ci/win-x64/setup-qt.sh

# I'm missing some paths here
export Qt5_DIR=C:/Qt/Qt5.13.1/5.13.1/msvc2017_64/lib/cmake/Qt5

cmake .. ${cmake_args} \
    -DMOMIJI_BUILD_TOOLS=1 \
    -DMOMIJI_BUILD_QT_GUI=1 \
    -DMOMIJI_BUILD_GL_GUI=1

${build_command}
