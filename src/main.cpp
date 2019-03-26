#include <iostream>
#include <string>

#include "Parser.h"
#include "System.h"

#include "Emulator.h"

void print_sys(const momiji::system& sys)
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

    std::cout << "\nlabels: ";
    for (const auto& label : sys.labels)
    {
        std::cout << label.name_hash << ' ';
    }
}

int main()
{
#if 0
    std::vector<momiji::system> cpu_states(1);

    bool should_loop = true;
    while (should_loop)
    {
        std::cout << ">> ";
        std::string str;
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
                print_sys(cpu_states.back());
                std::cout << '\n';
                break;

            case 'q':
                should_loop = false;
                break;

            case 'b':
                if (cpu_states.size() > 1)
                {
                    cpu_states.pop_back();
                }
                else
                {
                    std::cout << "Can't go back!\n";
                }
                break;

            case 'c':
                std::cout << "Changes between runs: \n";
                for (const auto& x : cpu_states)
                {
                    print_sys(x);
                    std::cout << "\n---\n";
                }
            }

            continue;
        }

        auto instr = momiji::readInstruction(str);

        if (!instr.has_value())
        {
            momiji::parser_error& error = instr.error();
            std::cout << "Error at " << error.line << ':' << error.column << ", ";
            switch (error.errorType)
            {
            case momiji::parser_error::error_type::NoInstructionFound:
                std::cout << "no instruction found.\n";
                break;

            case momiji::parser_error::error_type::UnexpectedCharacter:
                std::cout << "unexpected character.\n";
                break;

            case momiji::parser_error::error_type::WrongInstruction:
                std::cout << "no such instruction.\n";
                break;

            case momiji::parser_error::error_type::WrongOperandType:
                std::cout << "wrong operand type.\n";
                break;
            }

            continue;
        }

        momiji::instruction unboxed_instr = instr.value();

        cpu_states.emplace_back(unboxed_instr.executefn(cpu_states.back(), unboxed_instr));

        print_sys(cpu_states.back());


        std::cout << "\n";
    }
#else

    momiji::emulator emu;

    bool should_loop = true;
    while (should_loop)
    {
        std::cout << ">> ";
        std::string str;
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
            case 'n':
                emu.step();
            }

            continue;
        }

        auto instr = emu.parse(str);

        if (instr.has_value())
        {
            momiji::parser_error& error = instr.value();
            std::cout << "Error at " << error.line << ':' << error.column << ", ";
            switch (error.errorType)
            {
            case momiji::parser_error::error_type::NoInstructionFound:
                std::cout << "no instruction found.\n";
                break;

            case momiji::parser_error::error_type::UnexpectedCharacter:
                std::cout << "unexpected character.\n";
                break;

            case momiji::parser_error::error_type::WrongInstruction:
                std::cout << "no such instruction.\n";
                break;

            case momiji::parser_error::error_type::WrongOperandType:
                std::cout << "wrong operand type.\n";
                break;
            }

            continue;
        }


        print_sys(emu.getStates().back());

        std::cout << "\n";
    }

#endif
}
