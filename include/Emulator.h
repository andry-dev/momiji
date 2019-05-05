#pragma once

#include <vector>
#include <string>
#include "System.h"
#include "Parser.h"
#include <optional>

namespace momiji
{
    struct Emulator
    {
    private:
        std::vector<momiji::System> systemStates;
        std::vector<momiji::Instruction> instructions;

    public:
        Emulator();

        const std::vector<momiji::System>& getStates() const;
        const std::vector<momiji::Instruction>& getInstructions() const;

        std::optional<momiji::ParserError> parse(const std::string& str);
        bool rollback();
        bool step();
    };
}
