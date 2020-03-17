cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0069 NEW)

project(momiji CXX)

option(MOMIJI_BUILD_GL_GUI "Enable OpenGL GUI support (requires tewi)" OFF)
option(MOMIJI_BUILD_QT_GUI "Enable Qt GUI support" OFF)
option(MOMIJI_BUILD_TOOLS "Build CLI tools" OFF)
option(MOMIJI_BUILD_DOC "Build documentation with xelatex" OFF)
option(MOMIJI_BUILD_TESTS "Build tests" OFF)
option(MOMIJI_ENABLE_LTO "Enable LTO for all the modules" ON)
option(MOMIJI_USE_ASAN "Compile with AddressSanitizer" OFF)
option(MOMIJI_USE_UBSAN "Compile with UndefinedBehaviourSanitizer" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

include(cmake/set-flags.cmake)

find_package(Git)

execute_process(COMMAND
    ${GIT_EXECUTABLE} describe --always --dirty
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE MOMIJI_GIT_SHA1
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
    ${GIT_EXECUTABLE} rev-list --count HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE MOMIJI_GIT_REV
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

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

if (UNIX AND NOT APPLE)
    include(cmake/linux-install.cmake)
endif()

# Packaging options
set(CPACK_GENERATOR "7Z")

if (WIN32)
    list(APPEND CPACK_GENERATOR "NSIS")
elseif (APPLE)
    list(APPEND CPACK_GENERATOR "DragNDrop")
endif()

include(CPack)
include(CPackComponent)

cpack_add_component(libmomiji
    DISPLAY_NAME "Main library (static)"
    REQUIRED
)

if (MOMIJI_BUILD_TOOLS)
    cpack_add_component(tools
        DISPLAY_NAME "CLI tools for Momiji"
        DEPENDS libmomiji
    )
endif()

if (MOMIJI_BUILD_GL_GUI)
    cpack_add_component(gl
        DISPLAY_NAME "OpenGL GUI for Momiji"
        DEPENDS libmomiji
    )
endif()

if (MOMIJI_BUILD_QT_GUI)
    cpack_add_component(qt
        DISPLAY_NAME "Qt5 GUI for Momiji"
        DEPENDS libmomiji
    )
endif()
