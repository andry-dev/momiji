#include "move.h"

#include <cstddef>
#include <cstdint>

#include "../Instructions/OpCodes.h"
#include "./Utils.h"
#include <Memory.h>
#include <Parser.h>
#include <Utils.h>

#include "../Instructions/Representations.h"

namespace momiji::enc
{
    void move(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::Move bits;
        bits.header = 0b00;

        // This Move requires special handling of sizes.
        const auto size =
            std::int8_t(move_sizeconv[utils::to_val(instr.dataType)]);

        // Byte size
        bits.size = size & 0b011;

        // Destination
        bits.dsttype = getCorrectOpType(instr.operands[1]);
        bits.dstmode = getCorrectOpMode(instr.operands[1]);

        bits.srctype = getCorrectOpType(instr.operands[0]);
        bits.srcmode = getCorrectOpMode(instr.operands[0]);

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 14) | (bits.size << 12) |
                                   (bits.dstmode << 9) | (bits.dsttype << 6) |
                                   (bits.srctype << 3) | (bits.srcmode));
    }
} // namespace momiji::enc
