cmake_minimum_required(VERSION 3.8)

add_executable(momiji-gl src/main.cpp)
target_compile_features(momiji-gl PRIVATE cxx_std_17)
target_link_libraries(momiji-gl PRIVATE libmomiji)

find_package(tewi QUIET)

if (NOT TARGET tewi::tewi)
    message(STATUS "[momiji] Using local tewi repo")
    add_subdirectory(external/tewi)
    add_library(tewi::tewi ALIAS tewi)
endif()

target_sources(momiji-gl PRIVATE src/Gui.cpp src/Renderer.cpp)
target_link_libraries(momiji-gl PRIVATE tewi::tewi)
target_compile_definitions(momiji-gl PRIVATE MOMIJI_INCLUDE_GUI=1 TEWI_STATIC_LIB)

add_custom_command(TARGET momiji-gl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/postbuild
    $<TARGET_FILE_DIR:momiji-gl>)

install(TARGETS momiji-gl)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/postbuild/res DESTINATION bin/)
