#include "utils.h"
#include <momiji/System.h>

int main(int argc, const char** argv)
{
    auto args = utils::convArgs(argc, argv);

    if (args.size() == 0)
    {
        return 1;
    }

    return 0;
}
