#pragma once

#include "Common.h"
#include "Instructions.h"

#include <Utils.h>

#include <array>

namespace momiji
{
    struct MappingType
    {
        std::uint32_t mapping;
        details::parserfn_t execfn;
    };

    constexpr std::array<MappingType, 42> mappings = { {
        { utils::hash("move"), details::parseMove },
        { utils::hash("moveq"), details::parseMoveQ },

        { utils::hash("add"), details::parseAdd },
        { utils::hash("adda"), details::parseAdd },
        { utils::hash("addi"), details::parseAdd },

        { utils::hash("sub"), details::parseSub },
        { utils::hash("suba"), details::parseSub },
        { utils::hash("subi"), details::parseSub },

        { utils::hash("muls"), details::parseMuls },
        { utils::hash("mulu"), details::parseMulu },

        { utils::hash("divs"), details::parseDivs },
        { utils::hash("divu"), details::parseDivu },

        { utils::hash("swap"), details::parseSwap },
        { utils::hash("exg"), details::parseExg },

        { utils::hash("or"), details::parseOr },
        { utils::hash("ori"), details::parseOr },
        { utils::hash("and"), details::parseAnd },
        { utils::hash("andi"), details::parseAnd },
        { utils::hash("eor"), details::parseXor },
        { utils::hash("eori"), details::parseXor },
        { utils::hash("not"), details::parseNot },

        { utils::hash("cmp"), details::parseCmp },
        { utils::hash("cmpi"), details::parseCmp },
        { utils::hash("cmpa"), details::parseCmp },
        { utils::hash("tst"), details::parseTst },

        { utils::hash("jmp"), details::parseJmp },
        { utils::hash("jsr"), details::parseJsr },
        { utils::hash("bra"), details::parseBra },
        { utils::hash("bsr"), details::parseBsr },
        { utils::hash("rts"), details::parseRts },

        { utils::hash("blt"), details::parseBlt },
        { utils::hash("ble"), details::parseBle },
        { utils::hash("bge"), details::parseBge },
        { utils::hash("bgt"), details::parseBgt },
        { utils::hash("beq"), details::parseBeq },
        { utils::hash("bne"), details::parseBne },

        { utils::hash("asl"), details::parseAsl },
        { utils::hash("asr"), details::parseAsr },
        { utils::hash("lsl"), details::parseLsl },
        { utils::hash("lsr"), details::parseLsr },

        // Extensions
        { utils::hash("dc"), details::parseDeclare },
        { utils::hash("hcf"), details::parseHcf }
    } };
} // namespace momiji
