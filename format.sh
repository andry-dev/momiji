#!/bin/bash

find include -iname *.h | xargs clang-format -i -style=file
find src -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file
