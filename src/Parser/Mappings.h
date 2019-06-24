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


    constexpr std::array<MappingType, 29> mappings = {{
        { utils::hash("move"),     parseMove },
        { utils::hash("moveq"),    parseMoveQ },

        { utils::hash("add"),      parseAdd },
        { utils::hash("adda"),     parseAdd },
        { utils::hash("addi"),     parseAdd },

        { utils::hash("sub"),      parseSub },
        { utils::hash("suba"),     parseSub },
        { utils::hash("subi"),     parseSub },

        { utils::hash("muls"),     parseMuls },
        { utils::hash("mulu"),     parseMulu },

        { utils::hash("divs"),     parseDivs },
        { utils::hash("divu"),     parseDivu },

        { utils::hash("swap"),     parseSwap },
        { utils::hash("exg"),      parseExg },

        { utils::hash("or"),       parseOr },
        { utils::hash("ori"),      parseOr },
        { utils::hash("and"),      parseAnd },
        { utils::hash("andi"),     parseAnd },
        { utils::hash("cmp"),      parseCmp },
        { utils::hash("cmpi"),     parseCmp },
        { utils::hash("cmpa"),     parseCmp },

        { utils::hash("jmp"),      parseJmp },
        { utils::hash("bra"),      parseBra },

        { utils::hash("blt"),      parseBlt },
        { utils::hash("ble"),      parseBle },
        { utils::hash("bge"),      parseBge },
        { utils::hash("bgt"),      parseBgt },
        { utils::hash("beq"),      parseBeq },
        { utils::hash("bne"),      parseBne },
    }};
}
