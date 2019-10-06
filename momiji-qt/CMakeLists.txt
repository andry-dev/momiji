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
    LinguistTools)

qt5_add_resources(RCCFiles
    res/resources.qrc)

qt5_create_translation(QmFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    res/translations/momijiqt_it.ts)

add_executable(momiji-qt
    MACOSX_BUNDLE WIN32
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.ui
    src/MemoryModel.cpp
    src/helpwindow.cpp
    src/helpwindow.ui
    src/aboutdialog.cpp
    src/aboutdialog.ui
    ${RCCFiles}
    ${QmFiles})

target_link_libraries(momiji-qt PUBLIC
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    libmomiji)

if (WIN32)
    target_link_libraries(momiji-qt PUBLIC
        Qt5::WinMain
    )
endif()

momiji_set_target_flags(momiji-qt)

add_custom_command(TARGET momiji-qt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_CURRENT_BINARY_DIR}/translations

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/momijiqt_it.qm
        ${CMAKE_CURRENT_BINARY_DIR}/translations)

include(cmake/DeployQt.cmake)

if (WIN32)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION momiji-qt)
    windeployqt(momiji-qt)
    install(TARGETS momiji-qt
            DESTINATION momiji-qt
            COMPONENT qt)

    install(DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}/translations
            ${CMAKE_CURRENT_BINARY_DIR}/platforms
            ${CMAKE_CURRENT_BINARY_DIR}/iconengines
            ${CMAKE_CURRENT_BINARY_DIR}/imageformats
            ${CMAKE_CURRENT_BINARY_DIR}/styles
            DESTINATION  momiji-qt)
elseif (APPLE)
    macdeployqt(momiji-qt)

    install(TARGETS momiji-qt
            BUNDLE DESTINATION .
            COMPONENT qt)

    # ?
elseif (UNIX AND NOT APPLE) # Linux
    install(TARGETS momiji-qt
            COMPONENT qt)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations
            DESTINATION  share/momiji-qt/)

    install(FILES deploy/momiji-qt.desktop
            DESTINATION share/applications)
endif()
