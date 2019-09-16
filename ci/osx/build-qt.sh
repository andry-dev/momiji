#!/bin/bash


brew install qt

# Investigate paths:
# https://github.com/robotology/robotology-superbuild/issues/4
# Probably:
export Qt5_DIR=/usr/local/opt/qt5/lib/cmake/Qt5

cmake .. ${cmake_args} -DMOMIJI_BUILD_QT_GUI=1

${build_command}
