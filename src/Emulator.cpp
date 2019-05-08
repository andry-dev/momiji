#include "Emulator.h"

#include <iterator>
#include <iostream>

namespace momiji
{
    Emulator::Emulator()
        : systemStates(1)
    {
    }

    const std::vector<momiji::System>& Emulator::getStates() const
    {
        return systemStates;
    }

    std::optional<momiji::ParserError> Emulator::newState(const std::string& str)
    {
        auto res = momiji::parse(str);

        if (res)
        {
            auto lastSys = systemStates.back();
            lastSys.instructions = std::move(*res);
            systemStates.emplace_back(std::move(lastSys));
        }

        return std::nullopt;
    }

    bool Emulator::rollbackSys()
    {
        if (systemStates.size() > 1)
        {
            systemStates.pop_back();
            return true;
        }

        return false;
    }

    bool Emulator::rollback()
    {
        auto& lastSys = systemStates.back();
        if (lastSys.cpu.programCounter.value > 0)
        {
            --lastSys.cpu.programCounter.value;

            return true;
        }

        return false;
    }

    bool Emulator::step()
    {
        auto& lastSys = systemStates.back();
        const std::int32_t pc = lastSys.cpu.programCounter.value;
        if (pc < 0 || pc >= lastSys.instructions.size())
        {
            return false;
        }

        const auto& instr = lastSys.instructions[pc];

        System newstate = {};//instr.executefn(lastSys, instr);

        switch (instr.instructionType)
        {
        case InstructionType::Jmp: [[fallthrough]];
        case InstructionType::BranchEqual: [[fallthrough]];
        case InstructionType::BranchGreaterEquals: [[fallthrough]];
        case InstructionType::BranchGreaterThan: [[fallthrough]];
        case InstructionType::BranchLessEquals: [[fallthrough]];
        case InstructionType::BranchLessThan: [[fallthrough]];
        case InstructionType::BranchNotEquals: [[fallthrough]];
        case InstructionType::BranchNotZero: [[fallthrough]];
        case InstructionType::BranchZero:
            break;

        default:
            ++newstate.cpu.programCounter.value;
        }


        systemStates.emplace_back(std::move(newstate));

        return true;
    }

    bool Emulator::reset()
    {
        bool ret = false;
        while (systemStates.size() > 1)
        {
            systemStates.pop_back();
            ret = true;
        }

        return ret;
    }
}
