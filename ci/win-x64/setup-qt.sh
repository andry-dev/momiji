#!/bin/bash

echo "--> aqtinstall"
pip3 install aqtinstall

mkdir /c/Qt

aqt install -O C:\Qt 5.13.1 windows desktop win64_msvc2017_64

echo "--> QT5 PATHS"

ls /c/Qt
ls /c/Qt/Qt5.13.1
ls /c/Qt/Qt5.13.1/5.13.1
