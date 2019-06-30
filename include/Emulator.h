#pragma once

#include <Parser.h>
#include <System.h>
#include <optional>
#include <string>
#include <vector>

namespace momiji
{
    struct EmulatorSettings
    {
        std::int32_t programStart = 0;
        std::int32_t dataSectionOffset = -1;

        // Never = Never share executable memory with previous system states
        // Guess = Copy the program memory only if the source code changes
        // Always = Always share executable memory with previous system states
        //
        // Use Never if you want to keep track of every possible state, at the
        // cost of losing track of the PC
        //
        // Use Guess if you want to copy the program memory only if there is a
        // "newState()" call
        //
        // Use Always if you want to always use the same program memory, even if
        // the source code changes. Use reset() to completely wipe out the
        // memory
        //
        // The copy of the system state will ALWAYS happen, changing any of
        // these options will not affect that.
        enum class ShareCompiledMemory : std::int8_t
        {
            Never,
            Guess,
            Always,
        } shareMemory = ShareCompiledMemory::Never;
    };

    struct Emulator
    {
      private:
        std::vector<momiji::System> systemStates;

      public:
        Emulator();
        Emulator(EmulatorSettings);

        const std::vector<momiji::System>& getStates() const;

        std::optional<momiji::ParserError> newState(const std::string& str);
        bool rollbackSys();
        bool rollback();
        bool step();
        bool reset();
    };
} // namespace momiji
