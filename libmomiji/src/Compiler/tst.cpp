#include "tst.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void tst(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>&)
    {

        repr::Tst bits;

        bits.size    = utils::to_val(instr.dataType);
        bits.regtype = utils::to_val(instr.operands[0].operandType);
        bits.regmode = getCorrectOpMode(instr, 0);

        opcode.val = (bits.header << 8) | (bits.size << 6) |
                     (bits.regmode << 3) | (bits.regtype);
    }
} // namespace momiji::enc
