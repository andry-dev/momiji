cmake_minimum_required(VERSION 3.13)

function(momiji_new_test test_name)
    add_executable(${test_name} ${ARGV1})
    target_link_libraries(${test_name} libmomiji)
    momiji_set_target_flags(${test_name})
endfunction()

momiji_new_test(parser-instr src/parser-instr.cpp)

add_test(NAME TestParserInstructions COMMAND parser-instr)
