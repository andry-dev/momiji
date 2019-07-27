#include "utils.h"
#include <bitset>
#include <filesystem>
#include <momiji/Emulator.h>
#include <momiji/Memory.h>
#include <string_view>

static void hackyPrintBin(std::uint8_t num)
{

    constexpr std::array<std::string_view, 16> binTable = { { "0000",
                                                              "0001",
                                                              "0010",
                                                              "0011",
                                                              "0100",
                                                              "0101",
                                                              "0110",
                                                              "0111",
                                                              "1000",
                                                              "1001",
                                                              "1010",
                                                              "1011",
                                                              "1100",
                                                              "1101",
                                                              "1110",
                                                              "1111"

    } };

    std::printf("%s%s", binTable[num >> 4].data(), binTable[num & 0x0F].data());
}

constexpr std::string_view usage = "USAGE: momiji-dump input_file\n";

int main(int argc, const char** argv)
{
    auto args = utils::convArgs(argc, argv);

    if (args.size() != 1)
    {
        return 1;
    }

    auto binary = utils::readBinary(args[0]);

    momiji::EmulatorSettings settings;
    settings.retainStates = momiji::EmulatorSettings::RetainStates::Never;

    momiji::Emulator emu { settings };
    emu.newState(binary);

    while (emu.step())
    {
        // Intentionally blank
    }

    const auto& state = emu.getStates().back();

    std::printf("--- Executable Memory Dump ---\n");

    const momiji::ConstExecutableMemoryView memview = state.mem;

    for (int i = 0; i < state.mem.size(); i += 2)
    {
        std::uint8_t lower  = memview.read8(i);
        std::uint8_t higher = 0;

        if ((i + 1) < memview.size())
        {
            higher = memview.read8(i + 1);
        }

        std::printf("%p: %.2x %.2x ", (memview.begin() + i), higher, lower);
        hackyPrintBin(higher);
        std::printf(" ");
        hackyPrintBin(lower);
        std::printf("\n");
    }

    std::printf("\n--- Data registers ---\n");

    for (int i = 0; i < state.cpu.dataRegisters.size(); ++i)
    {
        const auto val = state.cpu.dataRegisters[i].value;
        std::printf("d%d: 0x%.8x %d\n", i, val, val);
    }

    std::printf("\n--- Address registers ---\n");
    for (int i = 0; i < state.cpu.addressRegisters.size(); ++i)
    {
        const auto val = state.cpu.addressRegisters[i].value;
        std::printf("a%d: 0x%.8x %d\n", i, val, val);
    }

    std::printf("\n--- Status register ---\n");
    std::printf("N: %d ", state.cpu.statusRegister.negative);
    std::printf("\tZ: %d ", state.cpu.statusRegister.zero);
    std::printf("\tV: %d ", state.cpu.statusRegister.overflow);
    std::printf("\tX: %d ", state.cpu.statusRegister.extend);
    std::printf("\tC: %d\n", state.cpu.statusRegister.carry);

    return 0;
}
