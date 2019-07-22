#include "rts.h"

#include "../Instructions/rts.h"
#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction rts(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        ret.data.op1 = OperandType::DataRegister;
        ret.data.op2 = OperandType::DataRegister;
        ret.exec     = instr::rts;

        return ret;
    }
} // namespace momiji::dec
