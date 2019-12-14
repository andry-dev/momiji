#!/bin/bash

../ci/win-x64/setup-qt.sh

# I'm missing some paths here
export Qt5_DIR=C:/Qt/5.13.1/msvc2017_64/lib/cmake/Qt5

cmake .. ${cmake_args} \
    -DMOMIJI_BUILD_QT_GUI=1

${build_command}
