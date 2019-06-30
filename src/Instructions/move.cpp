#include "move.h"

#include "Utils.h"

namespace momiji
{
    namespace instr
    {

        momiji::System move(momiji::System sys, const InstructionData& data)
        {
            // For data and address registers the value is already stored
            std::uint32_t srcval = utils::to_val(data.mod1);
            auto* pc = sys.cpu.programCounter.address;

            switch (data.op1)
            {
            case OperandType::Immediate:
                switch (data.mod1)
                {
                case SpecialAddressingMode::Immediate:
                    srcval = utils::readImmediateFromPC(pc, data.size);                   
                    break;

                case SpecialAddressingMode::AbsoluteLong:
                case SpecialAddressingMode::AbsoluteShort:
                    const std::uint32_t memoff = utils::readImmediateFromPC(pc, data.size);
                    srcval = utils::readFromMemory(sys.mem.data(), memoff, data.size);
                    break;
                }
                break;

            case OperandType::DataRegister:
                srcval = sys.cpu.dataRegisters[srcval].value;
                break;

            case OperandType::AddressRegister:
                srcval = sys.cpu.addressRegisters[srcval].value;
                break;
            }

            std::int32_t dstval = utils::to_val(data.mod2);
            std::int32_t* tmp = nullptr;

            switch (data.op2)
            {
            case OperandType::DataRegister:
                tmp = &sys.cpu.dataRegisters[dstval].value;
                break;

            case OperandType::AddressRegister:
                tmp = &sys.cpu.addressRegisters[dstval].value;
                break;

            default:
            case OperandType::Immediate:
                switch (data.mod2)
                {
                case SpecialAddressingMode::AbsoluteShort:
                case SpecialAddressingMode::AbsoluteLong:
                    const std::uint32_t memoff = utils::readImmediateFromPC(pc, data.size);
                    tmp = reinterpret_cast<std::int32_t*>(sys.mem.data() + memoff);
                    break;
                }
                break;
            }

            Expects(tmp, "Can't find an appropriate destination mode");

            switch (data.size)
            {
            case 1:
                *tmp = (*tmp & 0xFFFF'FF00) | (srcval & 0x0000'00FF);
                break;

            case 2:
                *tmp = (*tmp & 0xFFFF'0000) | (srcval & 0x0000'FFFF);
                break;

            case 4:
                *tmp = srcval;
                break;
            }

            return sys;
        }
    }
}
