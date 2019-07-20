#include <momiji/Emulator.h>

#include <iterator>

#include <momiji/Compiler.h>
#include <momiji/Decoder.h>

#include "Instructions/bcc.h"
#include "Instructions/bra.h"
#include "Instructions/jmp.h"

namespace momiji
{
    static std::uint32_t
    immediateIncrPC(const momiji::DecodedInstruction& instr, std::uint32_t pc)
    {
        switch (instr.data.size)
        {
        case 1:
        case 2:
            pc += 4;
            break;

        case 4:
            pc += 6;
            break;
        }

        return pc;
    }

    static bool isJumpInstr(const momiji::DecodedInstruction& instr)
    {
        if (instr.exec == instr::bra || instr.exec == instr::bcc ||
            instr.exec == instr::jmp)
        {
            return true;
        }

        return false;
    }

    static void handlePC(System& newstate, const DecodedInstruction& instr)
    {
        if (!isJumpInstr(instr))
        {
            bool pc_incremented = false;

            auto check_multibyte = [](OperandType op) {
                return op == OperandType::Immediate ||
                       op == OperandType::AddressOffset ||
                       op == OperandType::AddressIndex;
            };

            if (check_multibyte(instr.data.op1))
            {
                newstate.cpu.programCounter.address =
                    immediateIncrPC(instr, newstate.cpu.programCounter.address);

                pc_incremented = true;
            }

            if (check_multibyte(instr.data.op2))
            {
                newstate.cpu.programCounter.address =
                    immediateIncrPC(instr, newstate.cpu.programCounter.address);

                pc_incremented = true;
            }

            if (!pc_incremented)
            {
                newstate.cpu.programCounter.address += 2;
            }
        }
    }

    Emulator::Emulator()
        : m_systemStates(1)
        , m_settings({ 0, -1, EmulatorSettings::RetainStates::Always })
    {
    }

    Emulator::Emulator(EmulatorSettings settings)
        : m_systemStates(1)
        , m_settings(settings)
    {
    }

    const std::vector<momiji::System>& Emulator::getStates() const
    {
        return m_systemStates;
    }

    std::optional<momiji::ParserError>
    Emulator::newState(const std::string& str)
    {
        if (str.empty())
        {
            return std::nullopt;
        }

        auto res = momiji::parse(str, m_settings.parserSettings);

        if (res)
        {
            auto mem     = momiji::compile(*res);
            auto lastSys = m_systemStates.back();
            lastSys.mem  = std::move(mem);
            m_systemStates.emplace_back(std::move(lastSys));

            return std::nullopt;
        }

        return res.error();
    }

    void Emulator::newState(momiji::ExecutableMemory binary)
    {
        auto lastSys = m_systemStates.back();
        lastSys.mem  = std::move(binary);
        m_systemStates.emplace_back(std::move(lastSys));
    }

    bool Emulator::rollbackSys()
    {
        if (m_systemStates.size() > 1)
        {
            m_systemStates.pop_back();
            return true;
        }

        return false;
    }

    bool Emulator::rollback()
    {
        auto& lastSys                = m_systemStates.back();
        ExecutableMemoryView memview = lastSys.mem;
        const auto pc                = lastSys.cpu.programCounter.address;

        auto pcadd = memview.begin() + pc;

        if (pcadd > memview.begin())
        {
            --lastSys.cpu.programCounter.address;

            return true;
        }

        return false;
    }

    bool Emulator::step()
    {
        auto& lastSys = m_systemStates.back();

        if (lastSys.mem.empty())
        {
            return false;
        }

        const auto pc = lastSys.cpu.programCounter.address;
        auto memview  = momiji::make_memory_view(lastSys);

        auto pcadd = memview.begin() + pc;

        if (pcadd < memview.begin() || pcadd >= memview.end())
        {
            return false;
        }

        auto instr = momiji::decode(memview, pc);

        switch (m_settings.retainStates)
        {
        case EmulatorSettings::RetainStates::Never:
            return stepHandleMem(never_retain_states_tag {}, instr);

        case EmulatorSettings::RetainStates::Always:
            return stepHandleMem(always_retain_states_tag {}, instr);
        }

        return false;
    }

    bool Emulator::stepHandleMem(never_retain_states_tag,
                                 DecodedInstruction& instr)
    {
        auto& lastSys = m_systemStates.back();

        instr.exec(lastSys, instr.data);

        handlePC(lastSys, instr);

        return true;
    }

    bool Emulator::stepHandleMem(always_retain_states_tag,
                                 DecodedInstruction& instr)
    {
        auto& lastSys = m_systemStates.back();

        // Copy the new state
        auto newstate = lastSys;

        instr.exec(newstate, instr.data);

        handlePC(newstate, instr);

        m_systemStates.emplace_back(std::move(newstate));

        return true;
    }

    bool Emulator::reset()
    {
        bool ret = false;
        while (m_systemStates.size() > 1)
        {
            m_systemStates.pop_back();
            ret = true;
        }

        return ret;
    }

    void Emulator::loadNewSettings(EmulatorSettings settings)
    {
        reset();

        m_settings = settings;
    }
} // namespace momiji
