#pragma once

#include <Utils.h>

#include <Memory.h>
#include <System.h>

namespace momiji
{
    namespace utils
    {
        inline std::uint32_t readImmediateFromPC(ExecutableMemoryView base,
                                                 std::uint32_t pc,
                                                 std::int16_t size)
        {
            std::uint32_t val = 0;
    
            switch (size)
            {
            case 1:
                return base.read8(pc + 3);

            case 2:
                return base.read16(pc + 2);

            case 4:
                return base.read32(pc + 2);
            }

            return val;
        }

        inline std::uint32_t readFromMemory(ExecutableMemoryView base,
                                            std::uint32_t offset,
                                            std::int16_t size)
        {
            std::uint32_t val = 0;

            switch (size)
            {
            
            case 1:
            {
                const std::uint32_t correct_offset = utils::sign_extend<std::int8_t>(offset);
                return base.read8(correct_offset);
            } break;

            case 2:
            {
                const std::uint32_t correct_offset = utils::sign_extend<std::int16_t>(offset);
                return base.read16(correct_offset);
            } break;

            case 4:
                return base.read32(offset);
                break;
            }


            return val;
        }
    }
}
