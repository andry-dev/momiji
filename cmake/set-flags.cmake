include(CheckIPOSupported)
check_ipo_supported(RESULT MOMIJI_HAVE_IPO)

function(momiji_set_target_flags target)
    if (MOMIJI_HAVE_IPO AND MOMIJI_ENABLE_LTO)
        message("[momiji] LTO enabled for ${target}")
        set_property(TARGET ${target}
            PROPERTY
            INTERPROCEDURAL_OPTIMIZATION ON)
    endif()

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
    )

    set(COMMON_GCC_FLAGS "-Wall -Wpedantic -pedantic -Wshadow -Werror")
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GCC")
        target_compile_options(${target} PUBLIC
            -Wall -Wpedantic -pedantic -Werror)

    elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        target_compile_options(${target} PUBLIC
            -Wall -Wpedantic -pedantic -Werror
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-padded
            -Wno-weak-vtables
            -Wno-missing-noreturn
            -Wno-extra-semi-stmt
            -Wno-shadow-field-in-constructor
            -Wno-missing-braces
            -Wno-switch-enum
            -Wno-shadow
            #-Wno-sign-conversion
            #-Wno-conversion
            -Wno-exit-time-destructors
            -Wno-global-constructors
            -Wno-format-pedantic
            -Wno-redundant-parens
            -Wno-used-but-marked-unused
            -ferror-limit=5
        )
    endif()
endfunction()
