#!/bin/bash

find libmomiji/include -iname *.h | xargs clang-format -i -style=file
find libmomiji/src -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file

find momiji-gl/src -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file

find momiji-qt/src -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file

find momiji-tools/src -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file
