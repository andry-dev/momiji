cmake_minimum_required(VERSION 3.8)

project(momiji)

option(MOMIJI_BUILD_GUI "Enable GUI support (requires tewi)" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_BINARY_DIR})
add_executable(momiji src/main.cpp src/Parser.cpp src/Emulator.cpp)

target_compile_features(momiji PUBLIC cxx_std_17)

target_include_directories(momiji PUBLIC
                            ${PROJECT_SOURCE_DIR}/include
                            ${PROJECT_SOURCE_DIR}/external/nonstd)
if (MOMIJI_BUILD_GUI)
    find_package(tewi QUIET)

    if (NOT TARGET tewi::tewi)
        message(STATUS "[momiji] Found local tewi dir")
        add_subdirectory(external/tewi)
        add_library(tewi::tewi ALIAS tewi)
    endif()

    target_sources(momiji PRIVATE src/Gui.cpp src/Renderer.cpp)
    target_link_libraries(momiji PUBLIC tewi::tewi)
    target_compile_definitions(momiji PRIVATE MOMIJI_INCLUDE_GUI=1)
endif()

add_custom_command(TARGET momiji POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/postbuild
    $<TARGET_FILE_DIR:momiji>)
