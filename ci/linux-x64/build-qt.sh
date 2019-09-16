#!/bin/bash

cmake .. ${cmake_args} -DMOMIJI_BUILD_QT_GUI=1

${build_command}
