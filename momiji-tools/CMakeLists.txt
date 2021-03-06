cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0069 NEW)

function(new_tool target_name)
    add_executable(${target_name} ${ARGV1})
    target_link_libraries(${target_name} libmomiji)
    momiji_set_target_flags(${target_name})
endfunction()

new_tool(momiji-dump src/dump.cpp)
new_tool(momiji-as src/assembler.cpp)
new_tool(momiji-diff src/diff.cpp)


if (WIN32)
    install(TARGETS momiji-dump momiji-as momiji-diff
            DESTINATION momiji-tools
            COMPONENT tools)

elseif (UNIX AND NOT APPLE)
    install(TARGETS momiji-dump momiji-as momiji-diff
            COMPONENT tools)

    install(FILES
            deploy/momiji-as.desktop
            deploy/momiji-dump.desktop
            deploy/momiji-diff.desktop
            DESTINATION share/applications)
endif()
