include(CheckIPOSupported)
check_ipo_supported(RESULT MOMIJI_HAVE_IPO)

function(momiji_set_target_flags target)
    if (MOMIJI_HAVE_IPO)
        message("[momiji] LTO enabled for ${target}")
        set_property(TARGET ${target}
            PROPERTY
            INTERPROCEDURAL_OPTIMIZATION ON)
    endif()


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
            -Wno-format-pedantic
            -ferror-limit=5
            )
    endif()
endfunction()
