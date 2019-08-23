#pragma once

#include "Common.h"
#include "Directives.h"
#include "Instructions.h"

#include <momiji/Utils.h>

#include <array>

namespace momiji
{
    struct MappingType
    {
        std::uint32_t mapping;
        momiji::details::parserfn_t execfn;
    };

    constexpr std::array<MappingType, 1> directiveMappings = {
        { { utils::hash("section"), momiji::details::parseSection }

        }
    };

    constexpr std::array<MappingType, 42> mappings = {
        { { utils::hash("move"), momiji::details::parseMove },
          { utils::hash("moveq"), momiji::details::parseMoveQ },

          { utils::hash("add"), momiji::details::parseAdd },
          { utils::hash("adda"), momiji::details::parseAdd },
          { utils::hash("addi"), momiji::details::parseAdd },

          { utils::hash("sub"), momiji::details::parseSub },
          { utils::hash("suba"), momiji::details::parseSub },
          { utils::hash("subi"), momiji::details::parseSub },

          { utils::hash("muls"), momiji::details::parseMuls },
          { utils::hash("mulu"), momiji::details::parseMulu },

          { utils::hash("divs"), momiji::details::parseDivs },
          { utils::hash("divu"), momiji::details::parseDivu },

          { utils::hash("swap"), momiji::details::parseSwap },
          { utils::hash("exg"), momiji::details::parseExg },

          { utils::hash("or"), momiji::details::parseOr },
          { utils::hash("ori"), momiji::details::parseOr },
          { utils::hash("and"), momiji::details::parseAnd },
          { utils::hash("andi"), momiji::details::parseAnd },
          { utils::hash("eor"), momiji::details::parseXor },
          { utils::hash("eori"), momiji::details::parseXor },
          { utils::hash("not"), momiji::details::parseNot },

          { utils::hash("cmp"), momiji::details::parseCmp },
          { utils::hash("cmpi"), momiji::details::parseCmp },
          { utils::hash("cmpa"), momiji::details::parseCmp },
          { utils::hash("tst"), momiji::details::parseTst },

          { utils::hash("jmp"), momiji::details::parseJmp },
          { utils::hash("jsr"), momiji::details::parseJsr },
          { utils::hash("bra"), momiji::details::parseBra },
          { utils::hash("bsr"), momiji::details::parseBsr },
          { utils::hash("rts"), momiji::details::parseRts },

          { utils::hash("blt"), momiji::details::parseBlt },
          { utils::hash("ble"), momiji::details::parseBle },
          { utils::hash("bge"), momiji::details::parseBge },
          { utils::hash("bgt"), momiji::details::parseBgt },
          { utils::hash("beq"), momiji::details::parseBeq },
          { utils::hash("bne"), momiji::details::parseBne },

          { utils::hash("asl"), momiji::details::parseAsl },
          { utils::hash("asr"), momiji::details::parseAsr },
          { utils::hash("lsl"), momiji::details::parseLsl },
          { utils::hash("lsr"), momiji::details::parseLsr },

          // Extensions
          { utils::hash("dc"), momiji::details::parseDeclare },
          { utils::hash("hcf"), momiji::details::parseHcf } }
    };
} // namespace momiji
