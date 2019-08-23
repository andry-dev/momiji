#include "utils.h"

#include <momiji/Emulator.h>
#include <momiji/System.h>
#include <momiji/Utils.h>

#include <string_view>

constexpr std::string_view usage =
    "USAGE: momiji-diff file1 file2\n"
    "Returns the diff of two programs executions.\n";

int main(int argc, const char** argv)
{
    auto args = utils::convArgs(argc, argv);

    if (args.size() != 2)
    {
        std::cout << usage << '\n';
        return 1;
    }

    auto memProg1 = utils::readBinary(args[0]);
    auto memProg2 = utils::readBinary(args[1]);

    momiji::EmulatorSettings settings;
    settings.retainStates = momiji::EmulatorSettings::RetainStates::Never;
    settings.stackSize    = momiji::utils::make_kb(1);

    momiji::Emulator emuProg1 { settings };
    momiji::Emulator emuProg2 { settings };

    emuProg1.newState(memProg1);
    emuProg2.newState(memProg2);

    while (emuProg1.step())
    {
        // Intentionally blank
    }

    while (emuProg2.step())
    {
        // Intentionally blank
    }

    const auto& state1 = emuProg1.getStates().back();
    const auto& state2 = emuProg2.getStates().back();

    std::printf("--- Executable Memory Dump ---\n");

    const momiji::ConstExecutableMemoryView memview1 = state1.mem;
    const momiji::ConstExecutableMemoryView memview2 = state2.mem;

    for (std::int64_t i = memview1.executableMarker.begin,
                      j = memview2.executableMarker.begin;
         i < memview1.executableMarker.end && j < memview2.executableMarker.end;
         ++i, ++j)
    {
        auto lower1  = memview1.read8(i);
        auto higher1 = 0;

        auto lower2  = memview2.read8(j);
        auto higher2 = 0;

        if ((i + 1) < memview1.executableMarker.end)
        {
            higher1 = memview1.read8(i + 1);
        }

        if ((j + 1) < memview2.executableMarker.end)
        {
            higher2 = memview2.read8(j + 1);
        }

        if (lower1 != lower2 || higher1 != higher2)
        {
            std::printf("%p: %.2x %.2x \t %p: %.2x %.2x\n",
                        (memview1.begin() + i),
                        higher1,
                        lower1,
                        (memview2.begin() + j),
                        higher2,
                        lower2);
        }
    }

    std::printf("\n--- Data registers ---\n");

    for (std::uint32_t i = 0; i < state1.cpu.dataRegisters.size(); ++i)
    {
        const auto val1 = state1.cpu.dataRegisters[i].value;
        const auto val2 = state2.cpu.dataRegisters[i].value;

        if (val1 != val2)
        {
            std::printf(
                "d%d: 0x%.8x %d \t 0x%.8x %d\n", i, val1, val1, val2, val2);
        }
    }

    std::printf("\n--- Address registers ---\n");

    for (std::uint32_t i = 0; i < state1.cpu.addressRegisters.size(); ++i)
    {
        const auto val1 = state1.cpu.addressRegisters[i].value;
        const auto val2 = state2.cpu.addressRegisters[i].value;

        if (val1 != val2)
        {
            std::printf(
                "d%d: 0x%.8x %d \t 0x%.8x %d\n", i, val1, val1, val2, val2);
        }
    }

    return 0;
}
