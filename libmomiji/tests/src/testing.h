#pragma once

#define MOMIJI_TEST_REQUIRE(expr)                       \
    if (!(expr))                                        \
    {                                                   \
        std::printf("Failed! Checked for " #expr "\n"); \
        return 0;                                       \
    }
