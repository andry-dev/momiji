cmake_minimum_required(VERSION 3.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(momiji-qt
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.ui)

target_link_libraries(momiji-qt Qt5::Widgets libmomiji)
