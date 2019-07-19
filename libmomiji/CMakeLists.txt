cmake_minimum_required(VERSION 3.8)

add_library(libmomiji)
target_compile_features(libmomiji PUBLIC cxx_std_17)
target_include_directories(libmomiji PUBLIC SYSTEM
                           include
                           external/nonstd)

target_sources(libmomiji
    PRIVATE

    src/Parser/Parser.cpp
    src/Parser/Instructions.cpp
    src/Compiler/Compiler.cpp
    src/Decoder/Decoder.cpp
    src/Decoder/move.cpp
    src/Decoder/add.cpp
    src/Decoder/sub.cpp
    src/Decoder/div.cpp
    src/Decoder/mul.cpp
    src/Decoder/cmp.cpp
    src/Decoder/tst.cpp
    src/Decoder/or.cpp
    src/Decoder/and.cpp
    src/Decoder/exg.cpp
    src/Decoder/bra.cpp
    src/Decoder/bcc.cpp
    src/Decoder/jmp.cpp
    src/Decoder/internal.cpp

    src/Instructions/move.cpp
    src/Instructions/add.cpp
    src/Instructions/sub.cpp
    src/Instructions/div.cpp
    src/Instructions/mul.cpp
    src/Instructions/cmp.cpp
    src/Instructions/tst.cpp
    src/Instructions/or.cpp
    src/Instructions/and.cpp
    src/Instructions/exg.cpp
    src/Instructions/bra.cpp
    src/Instructions/bcc.cpp
    src/Instructions/jmp.cpp
    src/Instructions/internal.cpp

    src/Emulator.cpp)

find_package(asl REQUIRED)

if (NOT TARGET asl::asl)
    message(STATUS "[libmomiji] Can't find asl")
endif()

target_link_libraries(libmomiji PUBLIC asl::asl)
set_target_properties(libmomiji PROPERTIES PREFIX "")
