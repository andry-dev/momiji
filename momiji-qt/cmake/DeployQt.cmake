# The MIT License (MIT)
#
# Copyright (c) 2018 Nathan Osman
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

find_package(Qt5Core REQUIRED)

# Retrieve the absolute path to qmake and then use that path to find
# the windeployqt and macdeployqt binaries
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
if(WIN32 AND NOT WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "windeployqt not found")
endif()

find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
if(APPLE AND NOT MACDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "macdeployqt not found")
endif()

# Add commands that copy the required Qt files to the same directory as the
# target after being built as well as including them in final installation
function(windeployqt target)

    # Run windeployqt immediately after build
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                --verbose 0
                --no-compiler-runtime
                --no-angle
                --no-opengl-sw
                $<$<CONFIG:Release>:--release>
                $<$<CONFIG:Debug>:--debug>
                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Qt..."
    )


    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
        ${CMAKE_CURRENT_BINARY_DIR}/Qt5Core.dll
        ${CMAKE_CURRENT_BINARY_DIR}/Qt5Gui.dll
        ${CMAKE_CURRENT_BINARY_DIR}/Qt5Svg.dll
        ${CMAKE_CURRENT_BINARY_DIR}/Qt5Widgets.dll
    )


    if (MSYS)
        get_filename_component(msys_path ${CMAKE_CXX_COMPILER} PATH)
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
            ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
            ${msys_path}/libbz2-1.dll
            ${msys_path}/libdouble-conversion.dll
            ${msys_path}/libfreetype-6.dll
            ${msys_path}/libgcc_s_seh-1.dll
            ${msys_path}/libglib-2.0-0.dll
            ${msys_path}/libgraphite2.dll
            ${msys_path}/libharfbuzz-0.dll
            ${msys_path}/libiconv-2.dll
            ${msys_path}/libicudt64.dll
            ${msys_path}/libicudtdd64.dll
            ${msys_path}/libicuin64.dll
            ${msys_path}/libicuindd64.dll
            ${msys_path}/libicuio64.dll
            ${msys_path}/libicuiodd64.dll
            ${msys_path}/libicutest64.dll
            ${msys_path}/libicutestdd64.dll
            ${msys_path}/libicutu64.dll
            ${msys_path}/libicutudd64.dll
            ${msys_path}/libicuuc64.dll
            ${msys_path}/libicuucdd64.dll
            ${msys_path}/libintl-8.dll
            ${msys_path}/libpcre-1.dll
            ${msys_path}/libpcre2-16-0.dll
            ${msys_path}/libpng16-16.dll
            ${msys_path}/libstdc++-6.dll
            ${msys_path}/libwinpthread-1.dll
            ${msys_path}/zlib1.dll
        )
        message(STATUS "Runtime libs: ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}")
    endif()
    # windeployqt doesn't work correctly with the system runtime libraries,
    # so we fall back to one of CMake's own modules for copying them over

    # Doing this with MSVC 2015 requires CMake 3.6+
    if((MSVC_VERSION VERSION_EQUAL 1900 OR MSVC_VERSION VERSION_GREATER 1900)
            AND CMAKE_VERSION VERSION_LESS "3.6")
        message(WARNING "Deploying with MSVC 2015+ requires CMake 3.6+")
    endif()

    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
    foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
        get_filename_component(filename "${lib}" NAME)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                copy_if_different "${lib}" \"$<TARGET_FILE_DIR:${target}>\"
            COMMENT "Copying ${filename}..."
        )
    endforeach()
endfunction()

# Add commands that copy the required Qt files to the application bundle
# represented by the target.
function(macdeployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            \"$<TARGET_FILE_DIR:${target}>/../..\"
            -always-overwrite
        COMMENT "Deploying Qt..."
    )
endfunction()

mark_as_advanced(WINDEPLOYQT_EXECUTABLE MACDEPLOYQT_EXECUTABLE)
