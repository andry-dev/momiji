#pragma once

#include "Common.h"
#include "Instructions.h"

#include <Utils.h>

#include <array>

namespace momiji
{
#if 0
    namespace v1
    {
        struct MappingType
        {
            std::uint32_t mapping;
            momiji::details::v1::parserfn_t execfn;
        };

        constexpr std::array<MappingType, 42> mappings = {
            { { utils::hash("move"), momiji::details::v1::parseMove },
              { utils::hash("moveq"), momiji::details::v1::parseMoveQ },

              { utils::hash("add"), momiji::details::v1::parseAdd },
              { utils::hash("adda"), momiji::details::v1::parseAdd },
              { utils::hash("addi"), momiji::details::v1::parseAdd },

              { utils::hash("sub"), momiji::details::v1::parseSub },
              { utils::hash("suba"), momiji::details::v1::parseSub },
              { utils::hash("subi"), momiji::details::v1::parseSub },

              { utils::hash("muls"), momiji::details::v1::parseMuls },
              { utils::hash("mulu"), momiji::details::v1::parseMulu },

              { utils::hash("divs"), momiji::details::v1::parseDivs },
              { utils::hash("divu"), momiji::details::v1::parseDivu },

              { utils::hash("swap"), momiji::details::v1::parseSwap },
              { utils::hash("exg"), momiji::details::v1::parseExg },

              { utils::hash("or"), momiji::details::v1::parseOr },
              { utils::hash("ori"), momiji::details::v1::parseOr },
              { utils::hash("and"), momiji::details::v1::parseAnd },
              { utils::hash("andi"), momiji::details::v1::parseAnd },
              { utils::hash("eor"), momiji::details::v1::parseXor },
              { utils::hash("eori"), momiji::details::v1::parseXor },
              { utils::hash("not"), momiji::details::v1::parseNot },

              { utils::hash("cmp"), momiji::details::v1::parseCmp },
              { utils::hash("cmpi"), momiji::details::v1::parseCmp },
              { utils::hash("cmpa"), momiji::details::v1::parseCmp },
              { utils::hash("tst"), momiji::details::v1::parseTst },

              { utils::hash("jmp"), momiji::details::v1::parseJmp },
              { utils::hash("jsr"), momiji::details::v1::parseJsr },
              { utils::hash("bra"), momiji::details::v1::parseBra },
              { utils::hash("bsr"), momiji::details::v1::parseBsr },
              { utils::hash("rts"), momiji::details::v1::parseRts },

              { utils::hash("blt"), momiji::details::v1::parseBlt },
              { utils::hash("ble"), momiji::details::v1::parseBle },
              { utils::hash("bge"), momiji::details::v1::parseBge },
              { utils::hash("bgt"), momiji::details::v1::parseBgt },
              { utils::hash("beq"), momiji::details::v1::parseBeq },
              { utils::hash("bne"), momiji::details::v1::parseBne },

              { utils::hash("asl"), momiji::details::v1::parseAsl },
              { utils::hash("asr"), momiji::details::v1::parseAsr },
              { utils::hash("lsl"), momiji::details::v1::parseLsl },
              { utils::hash("lsr"), momiji::details::v1::parseLsr },

              // Extensions
              { utils::hash("dc"), momiji::details::v1::parseDeclare },
              { utils::hash("hcf"), momiji::details::v1::parseHcf } }
        };
    } // namespace v1

#endif

    inline namespace v2
    {
        struct MappingType
        {
            std::uint32_t mapping;
            momiji::details::v2::parserfn_t execfn;
        };

        constexpr std::array<MappingType, 42> mappings = {
            { { utils::hash("move"), momiji::details::v2::parseMove },
              { utils::hash("moveq"), momiji::details::v2::parseMoveQ },

              { utils::hash("add"), momiji::details::v2::parseAdd },
              { utils::hash("adda"), momiji::details::v2::parseAdd },
              { utils::hash("addi"), momiji::details::v2::parseAdd },

              { utils::hash("sub"), momiji::details::v2::parseSub },
              { utils::hash("suba"), momiji::details::v2::parseSub },
              { utils::hash("subi"), momiji::details::v2::parseSub },

              { utils::hash("muls"), momiji::details::v2::parseMuls },
              { utils::hash("mulu"), momiji::details::v2::parseMulu },

              { utils::hash("divs"), momiji::details::v2::parseDivs },
              { utils::hash("divu"), momiji::details::v2::parseDivu },

              { utils::hash("swap"), momiji::details::v2::parseSwap },
              { utils::hash("exg"), momiji::details::v2::parseExg },

              { utils::hash("or"), momiji::details::v2::parseOr },
              { utils::hash("ori"), momiji::details::v2::parseOr },
              { utils::hash("and"), momiji::details::v2::parseAnd },
              { utils::hash("andi"), momiji::details::v2::parseAnd },
              { utils::hash("eor"), momiji::details::v2::parseXor },
              { utils::hash("eori"), momiji::details::v2::parseXor },
              { utils::hash("not"), momiji::details::v2::parseNot },

              { utils::hash("cmp"), momiji::details::v2::parseCmp },
              { utils::hash("cmpi"), momiji::details::v2::parseCmp },
              { utils::hash("cmpa"), momiji::details::v2::parseCmp },
              { utils::hash("tst"), momiji::details::v2::parseTst },

              { utils::hash("jmp"), momiji::details::v2::parseJmp },
              { utils::hash("jsr"), momiji::details::v2::parseJsr },
              { utils::hash("bra"), momiji::details::v2::parseBra },
              { utils::hash("bsr"), momiji::details::v2::parseBsr },
              { utils::hash("rts"), momiji::details::v2::parseRts },

              { utils::hash("blt"), momiji::details::v2::parseBlt },
              { utils::hash("ble"), momiji::details::v2::parseBle },
              { utils::hash("bge"), momiji::details::v2::parseBge },
              { utils::hash("bgt"), momiji::details::v2::parseBgt },
              { utils::hash("beq"), momiji::details::v2::parseBeq },
              { utils::hash("bne"), momiji::details::v2::parseBne },

              { utils::hash("asl"), momiji::details::v2::parseAsl },
              { utils::hash("asr"), momiji::details::v2::parseAsr },
              { utils::hash("lsl"), momiji::details::v2::parseLsl },
              { utils::hash("lsr"), momiji::details::v2::parseLsr },

              // Extensions
              { utils::hash("dc"), momiji::details::v2::parseDeclare },
              { utils::hash("hcf"), momiji::details::v2::parseHcf } }
        };
    } // namespace v2
} // namespace momiji
