#pragma once

#include <vector>
#include <string>
#include "System.h"
#include "Parser.h"
#include <optional>

namespace momiji
{
    struct emulator
    {
    private:
        std::vector<momiji::system> systemStates;
        std::vector<momiji::instruction> instructions;

    public:
        emulator();

        const std::vector<momiji::system>& getStates() const;
        const std::vector<momiji::instruction>& getInstructions() const;

        std::optional<momiji::parser_error> parse(const std::string& str);
        bool rollback();
        bool step();
    };
}
