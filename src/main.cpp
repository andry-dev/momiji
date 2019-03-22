#include <iostream>
#include <string>

#include "Parser.h"
#include "CPU.h"

void print_cpu(const momiji::cpu_t& cpu)
{

    std::cout << "add: ";
    for (const auto& reg : cpu.addressRegisters)
    {
        std::cout << reg.value << ' ';
    }


    std::cout << "\ndata: ";
    for (const auto& reg : cpu.dataRegisters)
    {
        std::cout  << reg.value << ' ';
    }
}

int main()
{
    std::vector<momiji::cpu_t> cpu_states(1);

    while (true)
    {
        std::cout << ">> ";
        std::string str;
        std::getline(std::cin, str);

        if (str.size() > 1 && str[0] == '!')
        {
            if (str[1] == 'x')
            {
                std::cout << std::hex;
            }

            if (str[1] == 'd')
            {
                std::cout << std::dec;
            }

            if (str[1] == 'p')
            {
                print_cpu(cpu_states.back());
                std::cout << '\n';
            }

            if (str[1] == 'q')
            {
                break;
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
