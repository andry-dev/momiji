cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0069 NEW)

function(new_tool target_name)
    add_executable(${target_name} ${ARGV1})
    target_link_libraries(${target_name} libmomiji)
    momiji_set_target_flags(${target_name})
endfunction()

new_tool(momiji-dump src/dump.cpp)
new_tool(momiji-as src/assembler.cpp)
#new_tool(momiji-exec src/exec.cpp)

install(TARGETS momiji-dump momiji-as)
