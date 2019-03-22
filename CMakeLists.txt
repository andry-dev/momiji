cmake_minimum_required(VERSION 3.8)

project(momiji)

add_executable(momiji src/main.cpp src/Parser.cpp)

target_compile_features(momiji PUBLIC cxx_std_17)

target_include_directories(momiji PUBLIC
                            ${PROJECT_SOURCE_DIR}/include
                            ${PROJECT_SOURCE_DIR}/external/nonstd)
