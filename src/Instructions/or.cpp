#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::instr
{
    momiji::System or_instr(momiji::System sys, const InstructionData& data)
    {
        std::int32_t* srcreg = nullptr;

        const std::uint8_t srcnum = utils::to_val(data.mod1);

        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcreg = &sys.cpu.dataRegisters[srcnum].value;
            break;

        case OperandType::AddressRegister:
            srcreg = &sys.cpu.addressRegisters[srcnum].value;
            break;
        }

        std::int32_t* dstreg = nullptr;

        const std::uint8_t dstnum = utils::to_val(data.mod2);

        switch (data.op2)
        {
        case OperandType::DataRegister:
            dstreg = &sys.cpu.dataRegisters[dstnum].value;
            break;

        case OperandType::AddressRegister:
            dstreg = &sys.cpu.addressRegisters[dstnum].value;
            break;
        }

        std::int32_t tmp = 0;

        switch (data.size)
        {
        case 1:
            tmp = (*srcreg) & 0x000000FF;
            *dstreg = (*dstreg & 0xFFFFFF00) |
                      ((*dstreg | tmp) & 0x000000FF);
            break;
        case 2:
            tmp = (*srcreg) & 0x0000FFFF;
            *dstreg = (*dstreg & 0xFFFF0000) |
                      ((*dstreg | tmp) & 0x0000FFFF);
            break;

        case 4:
            *dstreg = *dstreg | *srcreg;
            break;
        }

        return sys;
    }

    momiji::System ori(momiji::System sys, const InstructionData& instr)
    {
        return sys;
    }
}
