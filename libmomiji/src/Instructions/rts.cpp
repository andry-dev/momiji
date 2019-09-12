#include "rts.h"

// an hack
#include "bra.h"
#include "jmp.h"
#include <Decoder.h>

namespace momiji::instr
{
    static int handleJsrData(const DecodedInstruction& instr)
    {
        switch (instr.data.operandType[0])
        {
        case OperandType::AddressOffset:
            [[fallthrough]];
        case OperandType::AddressIndex:
            return 2;

        case OperandType::Immediate:
            switch (instr.data.addressingMode[0])
            {
            case SpecialAddressingMode::AbsoluteShort:
                return 2;

            case SpecialAddressingMode::AbsoluteLong:
                return 4;

            case SpecialAddressingMode::Immediate:
                return instr.data.size;

            default:
                return 0;
            }

        default:
            return 0;
        }
    }

    momiji::System rts(momiji::System& sys,
                       const momiji::InstructionData& /*instr*/)
    {
        auto& sp = sys.cpu.addressRegisters[7].value;
        auto& pc = sys.cpu.programCounter.address;

        pc = *sys.mem.read32(sp);
        sp += 4;

        auto retInstr = momiji::decode(sys.mem, pc);

        if (retInstr.exec == instr::bsr)
        {
            pc += 2;

            std::int16_t offset = utils::to_val(retInstr.data.operandType[0]);
            if (offset == 0)
            {
                pc += 2;
            }
        }
        else if (retInstr.exec == instr::jsr)
        {
            pc += std::uint32_t(2 + handleJsrData(retInstr));
        }

        return sys;
    }
} // namespace momiji::instr
