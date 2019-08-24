#include "rts.h"

#include "../Instructions/rts.h"
#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction rts(ConstExecutableMemoryView /*mem*/,
                           std::int64_t /*idx*/)
    {
        DecodedInstruction ret;

        ret.data.operandType[0] = OperandType::DataRegister;
        ret.data.operandType[1] = OperandType::DataRegister;

        ret.exec = instr::rts;

        ret.string = "rts";

        return ret;
    }
} // namespace momiji::dec
