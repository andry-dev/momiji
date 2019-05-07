#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>

#include "Parser.h"
#include "System.h"

#include "Emulator.h"

#include "Gui.h"


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

#ifdef MOMIJI_INCLUDE_GUI
    gui();
#endif
}
