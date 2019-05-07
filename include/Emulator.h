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

    public:
        Emulator();

        const std::vector<momiji::System>& getStates() const;

        std::optional<momiji::ParserError> newState(const std::string& str);
        bool rollbackSys();
        bool rollback();
        bool step();
        bool reset();
    };
}
