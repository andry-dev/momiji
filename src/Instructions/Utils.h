#pragma once

#include <Utils.h>


namespace momiji
{
    namespace utils
    {
        inline std::uint32_t readImmediateFromPC(std::uint16_t* pc,
                                                 std::int16_t size)
        {
            std::uint32_t val = 0;
    
            switch (size)
            {
            case 1:
            case 2:
                val = *(pc + 1);
                break;

            case 4:
                val = (*(pc + 1) << 16) | (*(pc + 2));
                break;
            }

            return val;
        }

        inline std::uint32_t readFromMemory(std::uint16_t* base,
                                            std::uint32_t offset,
                                             std::int16_t size)
        {
            std::uint32_t val = 0;

            std::uint32_t correct_offset = 0;

            switch (size)
            {
            case 1:
            case 2:
                correct_offset = utils::sign_extend<std::int16_t>(offset);
                break;

            case 4:
                correct_offset = offset;
                break;
            }

            val = (*(base + correct_offset + 1) << 16) | (*(base + correct_offset));

            return val;
        }
    }
}
