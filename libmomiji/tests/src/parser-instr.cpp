#include "./testing.h"
#include <momiji/Parser.h>

int testAdd();

int testAdd()
{
    auto res = momiji::parse("add.w #123, d0");
    MOMIJI_TEST_REQUIRE(res.has_value());

    const auto& val = *res;

    MOMIJI_TEST_REQUIRE(val.instructions.size() == 1);
    MOMIJI_TEST_REQUIRE(val.labels.empty());

    // MOMIJI_TEST_REQUIRE();

    return 1;
}

int main()
{
    return static_cast<int>(!testAdd());
}
