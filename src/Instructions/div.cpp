#include "div.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    momiji::System divs(momiji::System& sys, const InstructionData& data)
    {
        auto pc = sys.cpu.programCounter.address;
        const auto memview = momiji::make_memory_view(sys);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t srcval = utils::to_val(data.mod1);
        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcval = sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcval = sys.cpu.addressRegisters[srcval].value;
            break;

        case OperandType::Immediate:
            switch (data.mod1)
            {
            case SpecialAddressingMode::Immediate:
                srcval = memview.read16(pc + 2);
                break;
            }
            break;
        }

        srcval = utils::sign_extend<std::int16_t>(srcval);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        std::int32_t quot = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem = (dstval % srcval) & 0x0000'FFFF;

        sys.cpu.dataRegisters[dstreg].value = (rem << 16) | (quot);

        return sys;
    }

    momiji::System divu(momiji::System& sys, const InstructionData& data)
    {
        auto pc = sys.cpu.programCounter.address;
        const auto memview = momiji::make_memory_view(sys);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t srcval = utils::to_val(data.mod1);
        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcval = sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcval = sys.cpu.addressRegisters[srcval].value;
            break;

        case OperandType::Immediate:
            switch (data.mod1)
            {
            case SpecialAddressingMode::Immediate:
                srcval = memview.read16(pc + 2);
                break;
            }
            break;
        }

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        std::int32_t quot = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem = (dstval % srcval) & 0x0000'FFFF;

        sys.cpu.dataRegisters[dstreg].value = (rem << 16) | (quot);

        return sys;
    }
}
