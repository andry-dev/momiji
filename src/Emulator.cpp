#include "Emulator.h"

#include <iterator>

namespace momiji
{
    Emulator::Emulator()
        : systemStates(1)
    { }

    const std::vector<momiji::System>& Emulator::getStates() const
    {
        return systemStates;
    }

    const std::vector<momiji::Instruction>& Emulator::getInstructions() const
    {
        return instructions;
    }

    std::optional<momiji::ParserError> Emulator::parse(const std::string& str)
    {
        // Parse normal instructions
        auto instr = momiji::readInstruction(str);

        if (!instr)
        {
            // Label?
            auto label = momiji::readLabel(str);

            if (!label)
            {
                return label.error();
            }

            auto unboxed_label = label.value();
            unboxed_label.idx = std::distance(instructions.begin(), instructions.end());

            auto lastSysState = systemStates.back();
            lastSysState.labels.emplace_back(std::move(unboxed_label));
            systemStates.emplace_back(std::move(lastSysState));

            return std::nullopt;
        }

        auto unboxed_instr = instr.value();

        instructions.emplace_back(unboxed_instr);

        return std::nullopt;
    }

    bool Emulator::rollback()
    {
        if (systemStates.size() > 1)
        {
            systemStates.pop_back();

            if (instructions.size() > 0)
            {
                instructions.pop_back();
            }

            return true;
        }

        return false;
    }

    bool Emulator::step()
    {
        std::int32_t pc = systemStates.back().cpu.programCounter.value;
        if (pc >= instructions.size())
        {
            return false;
        }

        auto newstate = instructions[pc].executefn(systemStates.back(), instructions[pc]);


        switch (instructions[pc].instructionType)
        {
        case InstructionType::Jmp:
            break;

        default:
            ++newstate.cpu.programCounter.value;
        }


        systemStates.emplace_back(newstate);

        return true;
    }
}
