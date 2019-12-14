FROM ubuntu:19.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update && \
    apt-get -y --no-install-recommends install \
                         clang build-essential make git sudo \
                         libxrandr-dev libx11-dev libxext-dev libxi-dev \
                         libxt-dev libxmu-dev libxfont-dev libxinerama-dev \
                         libxv-dev libxpresent-dev libxfixes-dev libxaw7-dev \
                         libfs-dev libfontenc-dev libdmx-dev libxcursor-dev \
                         libxkbcommon-dev libxkbcommon-x11-dev \
                         ninja-build qtbase5-dev qtbase5-dev-tools \
                         qttools5-dev qttools5-dev-tools \
                         libqt5svg5 \
                         libglfw3-dev python3-pip python3-setuptools \
                         imagemagick wget libfuse2 file gpg qt5-default \
                         inkscape qt5-style-plugins \
                         qt5-xdgdesktopportal-platformtheme \
                         qt5-gtk-platformtheme \
    && rm -rf /var/lib/apt/lists/*


RUN pip3 install conan

ADD https://cmake.org/files/v3.15/cmake-3.15.3-Linux-x86_64.sh /cmake-3.15.3-Linux-x86_64.sh
RUN mkdir /opt/cmake && \
    sh /cmake-3.15.3-Linux-x86_64.sh --prefix=/opt/cmake --skip-license && \
    ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
