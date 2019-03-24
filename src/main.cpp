#include <iostream>
#include <string>

#include "Parser.h"
#include "CPU.h"

void print_cpu(const momiji::cpu_t& cpu)
{

    std::cout << "add: ";
    for (const auto& reg : cpu.addressRegisters)
    {
        std::cout << static_cast<std::int32_t>(reg.value) << ' ';
    }

    std::cout << "\ndata: ";
    for (const auto& reg : cpu.dataRegisters)
    {
        std::cout  << static_cast<std::int32_t>(reg.value) << ' ';
    }
}

int main()
{
    std::vector<momiji::cpu_t> cpu_states(1);

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
                print_cpu(cpu_states.back());
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
                    print_cpu(x);
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

        print_cpu(cpu_states.back());

#if 0
        {
            std::cout << "\nWould you want to go back? [y/n] ";
            char ans = 'n';

            std::cin >> ans;

            if (ans == 'y')
            {
                cpu_states.pop_back();
            }

            std::cin.get();
        }
#endif

        std::cout << "\n";
    }
}
