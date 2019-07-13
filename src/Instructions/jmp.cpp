#include "jmp.h"

#include "./Utils.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System jmp(momiji::System& sys, const InstructionData& data)
    {
        std::uint32_t jmpadd = 0;

        auto memview = momiji::make_memory_view(sys);

        if (data.op1 == OperandType::Immediate)
        {
            const auto pc = sys.cpu.programCounter.address;
            jmpadd = utils::readImmediateFromPC(memview, pc, 4);
        }
        else if (data.op1 == OperandType::Address)
        {
            // jmp (a0)
            const std::int32_t reg = utils::to_val(data.mod1);
            jmpadd = sys.cpu.addressRegisters[reg].value;
        }

        sys.cpu.programCounter.address = jmpadd;

        return sys;
    }
} // namespace momiji::instr
