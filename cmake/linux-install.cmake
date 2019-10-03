cmake_minimum_required(VERSION 3.13)

add_custom_target(linux_icons)

function(momiji_linux_generate_icons size)
    add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/gen_icons/program_${size}.png
        COMMAND
        convert program.png -resize ${size}x${size} ${PROJECT_BINARY_DIR}/gen_icons/program_${size}.png
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data
        VERBATIM
        COMMENT "Creating image at size ${size}"
    )

    add_custom_target(
        gen-linux-logo-${size}
        ALL
        DEPENDS ${PROJECT_BINARY_DIR}/gen_icons/program_${size}.png
    )
endfunction()

function(momiji_linux_install_icons size)
    install(FILES ${PROJECT_BINARY_DIR}/gen_icons/program_${size}.png
            DESTINATION share/icons/hicolor/${size}x${size}/apps
            RENAME momiji.png)
endfunction()

momiji_linux_generate_icons(48)
momiji_linux_generate_icons(64)
momiji_linux_generate_icons(96)
momiji_linux_generate_icons(128)
momiji_linux_generate_icons(256)
momiji_linux_generate_icons(512)

momiji_linux_install_icons(48)
momiji_linux_install_icons(64)
momiji_linux_install_icons(96)
momiji_linux_install_icons(128)
momiji_linux_install_icons(256)
momiji_linux_install_icons(512)
