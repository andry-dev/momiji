#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>

#include "Parser.h"
#include "System.h"
#include "Compiler.h"
#include <Utils.h>

#include "Emulator.h"

#include "Gui.h"
#include <iomanip>

#include <bitset>


void print_sys(const momiji::System& sys)
{

    std::cout << "add: ";
    for (const auto& reg : sys.cpu.addressRegisters)
    {
        std::cout << static_cast<std::int32_t>(reg.value) << ' ';
    }

    std::cout << "\ndata: ";
    for (const auto& reg : sys.cpu.dataRegisters)
    {
        std::cout  << static_cast<std::int32_t>(reg.value) << ' ';
    }

    std::cout << "\npc: ";
    std::cout << sys.cpu.programCounter.value << ' ';

    std::cout << "N: ";
    std::cout << std::to_string(sys.cpu.statusRegister.negative) << ' ';
    std::cout << "Z: ";
    std::cout << std::to_string(sys.cpu.statusRegister.zero) << ' ';
    std::cout << "V: ";
    std::cout << std::to_string(sys.cpu.statusRegister.overflow) << ' ';
    std::cout << "X: ";
    std::cout << std::to_string(sys.cpu.statusRegister.extend) << ' ';
    std::cout << "C: ";
    std::cout << std::to_string(sys.cpu.statusRegister.carry) << ' ';
}

void cli()
{
    momiji::Emulator emu;

    bool should_loop = true;
    std::string str{};


    std::ifstream file{"file.asm"};

    std::string filestr(std::istreambuf_iterator<char>{file},
                        std::istreambuf_iterator<char>{});

    auto instr = emu.newState(filestr);

    while (should_loop)
    {
        std::cout << ">> ";

        std::getline(std::cin, str);

        if (str.size() > 1 && str[0] == '!')
        {
            switch (str[1])
            {
            case 'x':
                std::cout << std::hex;
                break;

            case 'd':
                std::cout << std::dec;
                break;

            case 'p':
                print_sys(emu.getStates().back());
                std::cout << '\n';
                break;

            case 'q':
                should_loop = false;
                break;

            case 'b':
                emu.rollback();
                break;

            case 'c':
                std::cout << "Changes between runs: \n";
                for (const auto& x : emu.getStates())
                {
                    print_sys(x);
                    std::cout << "\n---\n";
                }
                break;

            case 'n':
                emu.step();
                break;
            }

            continue;
        }

        if (instr.has_value())
        {
            momiji::ParserError& error = instr.value();
            std::cout << "Error at " << error.line << ':' << error.column << ", ";
            switch (error.errorType)
            {
            case momiji::ParserError::ErrorType::NoInstructionFound:
                std::cout << "no instruction found.\n";
                break;

            case momiji::ParserError::ErrorType::UnexpectedCharacter:
                std::cout << "unexpected character.\n";
                break;

            case momiji::ParserError::ErrorType::WrongInstruction:
                std::cout << "no such instruction.\n";
                break;

            case momiji::ParserError::ErrorType::WrongOperandType:
                std::cout << "wrong operand type.\n";
                break;

            case momiji::ParserError::ErrorType::NoLabelFound:
                std::cout << "no label found.\n";
                break;
            }

            continue;
        }

        print_sys(emu.getStates().back());

        std::cout << "\n";
    }
}


int main()
{
    //cli();

    std::string str{};


    std::ifstream file{"file.asm"};

    std::string filestr(std::istreambuf_iterator<char>{file},
                        std::istreambuf_iterator<char>{});


    auto res = momiji::parse(filestr);
    if (res)
    {
        auto instr = *res;
        auto binary = momiji::compile(instr);

        std::cout << "Base address: " << (void*)&binary[0] << '\n';
        int cnt = 0;
        for (int i = 0; i < binary.size(); i += 2)
        {
            std::cout << cnt << ": ";
            std::uint8_t higher = (binary[i]);
            std::uint8_t lower = (binary[i + 1]);
            std::bitset<8> bsh{higher};
            std::bitset<8> bsl{lower};
            std::cout << bsh << ' ' << bsl << ' ';

            if (i % 2 == 0)
            {
                std::cout << '\n';
            }
#if 0
            std::uint8_t group = (val >> 14);
            std::uint8_t size = (val >> 12) & 0b011;
            std::uint8_t dstr = (val >> 9) & 0b0111;
            std::uint8_t dstm = (val >> 6) & 0b0111;

            std::uint8_t srcm = (val >> 3) & 0b0111;
            std::uint8_t srcr = (val) & 0b0111;
            std::cout << "\tGroup " << std::to_string(group) << '\n';
            std::cout << "\tSize " << std::to_string(size) << '\n';
            std::cout << "\tDest Op " << std::to_string(dstr) << '\n';
            std::cout << "\tDest Mode " << std::to_string(dstm) << '\n';
            std::cout << "\tSrc Op " << std::to_string(srcr) << '\n';
            std::cout << "\tSrc Mode " << std::to_string(srcm) << '\n';

            if (srcr == momiji::utils::to_val(momiji::OperandType::Immediate) &&
                srcm == momiji::utils::to_val(momiji::SpecialAddressingMode::Immediate))
            {
                std::array<std::uint8_t, 4> move_conv{ 0b00, 0b01, 0b100, 0b010 };
                union
                {
                    std::int32_t val;
                    std::int16_t arr16[2];
                    std::int8_t arr8[4];
                };

                for (int j = 0; j < move_conv[size]; ++j)
                {
                    arr8[j] = binary[i + 2 + j];
                }

                std::cout << "Extratced size: " << std::to_string(move_conv[size]) << '\n';
                std::cout << "Extracted val: " << val << "\n";
                i += move_conv[size];
            }
#endif

            cnt += 2;
        }
    }

#ifdef MOMIJI_INCLUDE_GUI
    //gui();
#endif
}
