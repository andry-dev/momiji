cmake_minimum_required(VERSION 3.8)

project(momiji)

option(MOMIJI_BUILD_GUI "Enable GUI support (requires tewi)" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libmomiji)
target_compile_features(libmomiji PUBLIC cxx_std_17)
target_include_directories(libmomiji PUBLIC SYSTEM
                           ${PROJECT_SOURCE_DIR}/include
                           ${PROJECT_SOURCE_DIR}/external/nonstd)

target_sources(libmomiji
    PRIVATE

    src/RecParser.cpp
    src/Compiler/Compiler.cpp
    src/Decoder/Decoder.cpp
    src/Decoder/move.cpp
    src/Decoder/add.cpp
    src/Decoder/sub.cpp
    src/Decoder/div.cpp
    src/Decoder/mul.cpp
    src/Decoder/cmp.cpp
    src/Decoder/or.cpp
    src/Decoder/and.cpp
    src/Decoder/exg.cpp

    src/Instructions/move.cpp
    src/Instructions/add.cpp
    src/Instructions/sub.cpp
    src/Instructions/div.cpp
    src/Instructions/mul.cpp
    src/Instructions/cmp.cpp
    src/Instructions/or.cpp
    src/Instructions/and.cpp
    src/Instructions/exg.cpp

    src/Emulator.cpp)

find_package(asl REQUIRED)

if (NOT TARGET asl::asl)
    message(STATUS "[libmomiji] Can't find asl")
endif()

target_link_libraries(libmomiji PUBLIC asl::asl)
set_target_properties(libmomiji PROPERTIES PREFIX "")



add_executable(momiji src/main.cpp)
target_compile_features(momiji PRIVATE cxx_std_17)
target_link_libraries(momiji PRIVATE libmomiji)

if (MOMIJI_BUILD_GUI)
    find_package(tewi QUIET)

    if (NOT TARGET tewi::tewi)
        message(STATUS "[momiji] Found local tewi dir")
        add_subdirectory(external/tewi)
        add_library(tewi::tewi ALIAS tewi)
    endif()

    target_sources(momiji PRIVATE src/Gui.cpp src/Renderer.cpp)
    target_link_libraries(momiji PRIVATE tewi::tewi)
    target_compile_definitions(momiji PRIVATE MOMIJI_INCLUDE_GUI=1)
endif()

add_custom_command(TARGET momiji POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/postbuild
    $<TARGET_FILE_DIR:momiji>)
