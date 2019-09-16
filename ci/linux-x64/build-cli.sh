#!/bin/bash

cmake .. ${cmake_args} -DMOMIJI_BUILD_TOOLS=1

${build_command}
