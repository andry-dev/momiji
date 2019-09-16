#!/bin/bash

git submodule update --init --recursive ../momiji-gl/external/tewi

cd ../momiji-gl/external/tewi
conan create . andry/dev -b glfw -b stb -b glm -b tewi

cd ../../../build

conan install ..

cmake .. ${cmake_args} -DMOMIJI_BUILD_GL_GUI=1

${build_command}
