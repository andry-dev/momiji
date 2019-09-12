#include "tst.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void tst(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& /*labels*/,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>&)
    {

        repr::Tst bits;

        bits.size    = utils::to_val(instr.dataType);
        bits.regtype = getCorrectOpType(instr.operands[0]);
        bits.regmode = getCorrectOpMode(instr.operands[0]);

        opcode.val = std::uint16_t((bits.header << 8) | (bits.size << 6) |
                                   (bits.regmode << 3) | (bits.regtype));
    }
} // namespace momiji::enc
