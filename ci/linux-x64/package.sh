#!/bin/bash

cd /src/build


echo "--> Build dir"

pwd
ls

env DESTDIR=AppDir cmake --build . --target install

wget https://github.com/probonopd/linuxdeployqt/releases/download/6/linuxdeployqt-6-x86_64.AppImage -O linuxdeployqt

chmod +x linuxdeployqt

echo "--> Build dir"

ls

if [[ "${1}" != "lib" ]]; then
    for executable in AppDir/usr/share/applications/*.desktop; do
        rm -f AppDir/AppRun
        rm -f AppDir/*.desktop

        ./linuxdeployqt ${executable} \
            -appimage \
            -extra-plugins=iconengines,platformthemes/libqgtk3.so,platformthemes/libqt5ct.so \
            -unsupported-allow-new-glibc -unsupported-bundle-everything;
    done
fi

