cmake_minimum_required(VERSION 3.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

add_executable(momiji-qt
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.ui
    src/MemoryModel.cpp)

target_link_libraries(momiji-qt PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui libmomiji)

install(TARGETS momiji-qt)
