cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0069 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5
    COMPONENTS
    Widgets
    Core
    Gui
    LinguistTools
    REQUIRED)

qt5_add_resources(RCCFiles
    res/resources.qrc)

qt5_create_translation(QmFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    res/translations/momijiqt_it.ts)

add_executable(momiji-qt
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.ui
    src/MemoryModel.cpp
    src/helpwindow.cpp
    src/helpwindow.ui
    ${RCCFiles}
    ${QmFiles})

target_link_libraries(momiji-qt PUBLIC
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    libmomiji)

momiji_set_target_flags(momiji-qt)

add_custom_command(TARGET momiji-qt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_CURRENT_BINARY_DIR}/translations

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/momijiqt_it.qm
        ${CMAKE_CURRENT_BINARY_DIR}/translations)


include(cmake/DeployQt.cmake)
if (WIN32)
    windeployqt(momiji-qt)
elseif(APPLE)
    macdeployqt(momiji-qt)
endif()

install(TARGETS momiji-qt)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations
        DESTINATION  share/momiji-qt/)

