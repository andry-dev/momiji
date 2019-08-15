cmake_minimum_required(VERSION 3.8)

add_library(libmomiji)
target_compile_features(libmomiji PUBLIC cxx_std_17)

target_include_directories(libmomiji PUBLIC SYSTEM
                           include
                           external/nonstd)

target_include_directories(libmomiji PRIVATE SYSTEM include/momiji)

target_sources(libmomiji
    PRIVATE

    src/Parser/Parser.cpp
    #src/Parser/Parser2.cpp
    src/Parser/Instructions.cpp

    src/Compiler/Compiler.cpp
    src/Compiler/move.cpp
    src/Compiler/add.cpp
    src/Compiler/sub.cpp
    src/Compiler/div.cpp
    src/Compiler/mul.cpp
    src/Compiler/or.cpp
    src/Compiler/and.cpp
    src/Compiler/xor.cpp
    src/Compiler/not.cpp
    src/Compiler/cmp.cpp
    src/Compiler/tst.cpp
    src/Compiler/jmp.cpp
    src/Compiler/bra.cpp
    src/Compiler/bcc.cpp
    src/Compiler/rts.cpp
    src/Compiler/swap.cpp
    src/Compiler/exg.cpp
    src/Compiler/internal.cpp
    src/Compiler/Utils.cpp

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
    src/Decoder/not.cpp
    src/Decoder/exg.cpp
    src/Decoder/bra.cpp
    src/Decoder/bcc.cpp
    src/Decoder/jmp.cpp
    src/Decoder/rts.cpp
    src/Decoder/swap.cpp
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
    src/Instructions/rts.cpp
    src/Instructions/swap.cpp
    src/Instructions/noop.cpp
    src/Instructions/internal.cpp

    src/Emulator.cpp)

momiji_set_target_flags(libmomiji)

find_package(asl)

if (NOT TARGET asl::asl)
	message(STATUS "[momiji] Using local asl repo")
	add_subdirectory(external/asl)
	add_library(asl::asl ALIAS asl)
endif()

target_link_libraries(libmomiji PUBLIC asl::asl)
set_target_properties(libmomiji PROPERTIES
    PREFIX ""
    CXX_EXTENSIONS OFF)
