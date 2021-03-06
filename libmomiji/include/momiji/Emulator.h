#pragma once

#include <momiji/Decoder.h>
#include <momiji/Parser.h>
#include <momiji/System.h>

#include <optional>
#include <string>
#include <vector>

namespace momiji
{
    struct EmulatorSettings
    {
        std::int32_t programStart      = 0;
        std::int32_t dataSectionOffset = -1;

        std::int64_t stackSize = utils::make_kb(4);

        // Use Always to tell the emulator to always create new copies of the
        // system state for each instruction execution.
        //
        // Use Never to tell the emulator that every instruction should modify
        // the same system state.
        // Parsing the source code again _will_ create a new system state
        // anyway.
        enum class RetainStates : std::int8_t
        {
            Always,
            Never,
        } retainStates = RetainStates::Always;

        std::string toString()
        {
            switch (retainStates)
            {
            case RetainStates::Never:
                return "Never";

            case RetainStates::Always:
                return "Always";
            }

            return "";
        }

        ParserSettings parserSettings;
    };

    struct Emulator
    {
    private:
        std::vector<momiji::System> m_systemStates;
        EmulatorSettings m_settings;

        struct always_retain_states_tag
        {
        };
        struct never_retain_states_tag
        {
        };

        bool stepHandleMem(always_retain_states_tag, DecodedInstruction& instr);
        bool stepHandleMem(never_retain_states_tag, DecodedInstruction& instr);

    public:
        Emulator();
        Emulator(EmulatorSettings);

        [[nodiscard, deprecated("Use the states() function")]] const std::vector<momiji::System>& getStates() const;

        [[nodiscard]] gsl::span<momiji::System> states() noexcept;
        [[nodiscard]] gsl::span<const momiji::System> constStates() const noexcept;

        std::optional<momiji::ParserError> newState(const std::string& str);
        void newState(momiji::ExecutableMemory binary);
        bool rollback();
        bool step();
        bool reset();

        void loadNewSettings(EmulatorSettings);
        [[nodiscard]] EmulatorSettings getSettings() const noexcept;
    };

    template <typename F>
    void continueEmulatorExecution(Emulator& emu, F&& fun) noexcept
    {
        while (emu.step())
        {
            fun();
        }
    }

    void continueEmulatorExecution(Emulator& emu) noexcept;
} // namespace momiji
