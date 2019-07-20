cmake_minimum_required(VERSION 3.8)

project(momiji)

option(MOMIJI_BUILD_GL_GUI "Enable OpenGL GUI support (requires tewi)" OFF)
option(MOMIJI_BUILD_QT_GUI "Enable Qt GUI support" OFF)
option(MOMIJI_BUILD_TOOLS "Build CLI tools" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The library
add_subdirectory(libmomiji)

if (MOMIJI_BUILD_GL_GUI)
    add_subdirectory(momiji-gl)
endif()

if (MOMIJI_BUILD_QT_GUI)
    add_subdirectory(momiji-qt)
endif()

if (MOMIJI_BUILD_TOOLS)
    add_subdirectory(momiji-tools)
endif()
