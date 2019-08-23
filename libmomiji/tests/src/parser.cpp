#include <asl/testing>
#include <momiji/Parser.h>

int main()
{
    auto res = momiji::parse("add.w #123, d0");
    ASL_TEST_ASSERT(res.has_value());

    const auto& val = *res;
    ASL_TEST_ASSERT(val.instructions.size() == 1);
    ASL_TEST_ASSERT(val.labels.empty());

    ASL_TEST_ASSERT(false);

    return 0;
}
