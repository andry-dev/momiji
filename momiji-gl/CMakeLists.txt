cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0069 NEW)

add_executable(momiji-gl src/main.cpp)
target_compile_features(momiji-gl PRIVATE cxx_std_17)
target_link_libraries(momiji-gl PRIVATE libmomiji)

find_package(tewi REQUIRED)

target_sources(momiji-gl PRIVATE src/Gui.cpp src/Renderer.cpp)
target_link_libraries(momiji-gl PRIVATE tewi::tewi)
target_compile_definitions(momiji-gl PRIVATE MOMIJI_INCLUDE_GUI=1 TEWI_STATIC_LIB)
momiji_set_target_flags(momiji-gl)

add_custom_command(TARGET momiji-gl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/postbuild
    $<TARGET_FILE_DIR:momiji-gl>)

if (WIN32)
    install(TARGETS momiji-gl
            DESTINATION momiji-gl
            COMPONENT gl)

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/postbuild/res
            DESTINATION  momiji-gl/)
elseif(APPLE)
    macdeployqt(momiji-gl)

    install(TARGETS momiji-gl
            COMPONENT gl)

    # ?
elseif(UNIX AND NOT APPLE) # Linux
    install(TARGETS momiji-gl
            COMPONENT gl)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/postbuild/res
            DESTINATION  share/momiji-gl/)
endif()
