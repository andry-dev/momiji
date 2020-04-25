#include "utils.h"
#include <bitset>
#include <filesystem>
#include <string_view>

#include <momiji/Emulator.h>
#include <momiji/Memory.h>

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
        std::cout << usage;
        return 1;
    }

    auto binary = utils::readBinary(args[0]);

    if (!binary)
    {
        std::cout << "The file does not exist! Exiting.\n";
        return 1;
    }

    momiji::EmulatorSettings settings;
    settings.retainStates = momiji::EmulatorSettings::RetainStates::Never;
    settings.stackSize    = momiji::utils::make_kb(1);

    momiji::Emulator emu { settings };
    emu.newState(*binary);

    momiji::continueEmulatorExecution(emu);

    const auto& state = emu.constStates().back();

    std::printf("--- Executable Memory Dump ---\n");

    const momiji::ConstExecutableMemoryView memview = state.mem;

    for (std::int64_t i = memview.executableMarker.begin;
         i < memview.executableMarker.end;
         i += 2)
    {
        const std::uint8_t higher = memview.read8(i).value_or(0);
        const std::uint8_t lower  = memview.read8(i + 1).value_or(0);

        // Maybe unneeded now?
        /*
        if ((i + 1) < std::int64_t(memview.size()))
        {
            higher = memview.read8(i + 1).value_or(0);
        }
        */

        std::printf("%p: %.2x %.2x ", (memview.begin() + i), higher, lower);
        hackyPrintBin(higher);
        std::printf(" ");
        hackyPrintBin(lower);
        std::printf("\n");
    }

    std::printf("\n--- Data registers ---\n");

    for (std::uint32_t i = 0; i < state.cpu.dataRegisters.size(); ++i)
    {
        const auto val = state.cpu.dataRegisters[i].raw();
        std::printf("d%d: 0x%.8x %d\n", i, val, val);
    }

    std::printf("\n--- Address registers ---\n");
    for (std::uint32_t i = 0; i < state.cpu.addressRegisters.size(); ++i)
    {
        const auto val = state.cpu.addressRegisters[i].raw();
        std::printf("a%d: 0x%.8x %d\n", i, val, val);
    }

    std::printf("\n--- Status registers ---\n");
    std::printf("N: %d ", state.cpu.statusRegister.negative);
    std::printf("\tZ: %d ", state.cpu.statusRegister.zero);
    std::printf("\tV: %d ", state.cpu.statusRegister.overflow);
    std::printf("\tX: %d ", state.cpu.statusRegister.extend);
    std::printf("\tC: %d\n", state.cpu.statusRegister.carry);

    return 0;
}
