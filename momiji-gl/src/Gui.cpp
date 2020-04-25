#include "Gui.h"

#include <chrono>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tewi/Input/InputBuffer.hpp>
#include <tewi/Utils/ImGuiBindings.h>
#include <tewi/Utils/TickTimer.h>
#include <tewi/Video/BatchRenderer2D.hpp>
#include <tewi/Video/Renderer2D.hpp>
#include <tewi/Video/Shader.hpp>
#include <tewi/Video/Sprite.h>
#include <tewi/Video/Window.hpp>

#include "Renderer.h"
#include <momiji/Emulator.h>
#include <momiji/Utils.h>

#include <asl/types>
#include <cstdio>

namespace
{
    std::string toString(momiji::ParserOperand op)
    {
        switch (op)
        {
        case momiji::ParserOperand::DataRegister:
            return "Data register";

        case momiji::ParserOperand::AddressRegister:
            return "Address register";

        case momiji::ParserOperand::Address:
            return "Address";

        case momiji::ParserOperand::AddressPre:
            return "Address with pre-decrement";

        case momiji::ParserOperand::AddressPost:
            return "Address with post-increment";

        case momiji::ParserOperand::AddressOffset:
            return "Address with offset";

        case momiji::ParserOperand::AddressIndex:
            return "Indexed Address";

        case momiji::ParserOperand::Immediate:
            return "Immediate";

        case momiji::ParserOperand::AbsoluteShort:
            return "Absolute short";

        case momiji::ParserOperand::AbsoluteLong:
            return "Absolute long";

        case momiji::ParserOperand::ProgramCounterIndex:
            return "Indexed PC";

        case momiji::ParserOperand::ProgramCounterOffset:
            return "PC with offset";
        }

        return "???";
    }

    std::string toString(momiji::DataType op)
    {
        switch (op)
        {
        case momiji::DataType::Byte:
            return "Byte";

        case momiji::DataType::Word:
            return "Word";

        case momiji::DataType::Long:
            return "Long";
        }

        return "???";
    }
} // namespace

template <typename Container>
std::string contToString(const Container& cont)
{
    std::string res {};

    for (std::size_t i = 0; i < cont.size(); ++i)
    {
        res += toString(cont[i]);

        if (i != (cont.size() - 1))
        {
            res += ", ";
        }
    }

    return res;
}

void gui()
{
    using def_tag = tewi::API::OpenGLTag;

    momiji::EmulatorSettings emuSettings;

    emuSettings.retainStates = momiji::EmulatorSettings::RetainStates::Always;
    emuSettings.stackSize    = momiji::utils::make_kb(1);
    // emuSettings.parserSettings.breakpoints = gsl::null_span{};

    momiji::Emulator emu { emuSettings };

    tewi::Window<def_tag> win { "momiji",
                                tewi::Width { 1024 },
                                tewi::Height { 768 } };

    tewi::InputBuffer inputBuffer;
    win.bindTo(inputBuffer);

    bool lightTheme = true;

    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    tewi::initImGui(def_tag {}, win);

    bool showDebugInfo = false;

    constexpr std::chrono::duration<double, std::milli> fpsmax { 1000.0 /
                                                                 120.0 };

    std::string str;

    tewi::Sprite<def_tag> background { glm::vec2 { 0.0F, 0.0F },
                                       "res/bgimage.png" };

    momiji::renderer<def_tag> renderer {};

    auto shader = renderer.createShaderProgram();

    auto proj     = glm::ortho(0.0F, 1024.0F, 0.0F, 768.0F);
    glm::mat4 MVP = proj;

    while (!win.isClosed())
    {
        auto begintime = std::chrono::high_resolution_clock::now();

        win.clear();

        tewi::newFrameImGui(def_tag {}, win);
        ImGui::NewFrame();
        win.pollEvents(inputBuffer);

        if (lightTheme)
        {
            ImGui::StyleColorsLight();
        }
        else
        {
            ImGui::StyleColorsDark();
        }

        if (showDebugInfo)
        {
            ImGui::Begin("Debug menu");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0 / asl::f64(ImGui::GetIO().Framerate),
                        asl::f64(ImGui::GetIO().Framerate));

            ImGui::Separator();

            ImGui::BeginGroup();
            {
                {
                    ImGui::Text("sign_extend<i8>");
                    ImGui::SameLine();
                    static std::int32_t ext8 = 0;
                    ImGui::PushItemWidth(70.0F);
                    ImGui::InputInt("##1",
                                    &ext8,
                                    0,
                                    0,
                                    ImGuiInputTextFlags_CharsHexadecimal);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGui::Text("%d",
                                momiji::utils::sign_extend<std::int8_t>(ext8));
                }

                {
                    ImGui::Text("sign_extend<i16>");
                    ImGui::SameLine();
                    static std::int32_t ext16 = 0;
                    ImGui::PushItemWidth(70.0F);
                    ImGui::InputInt("##2",
                                    &ext16,
                                    0,
                                    0,
                                    ImGuiInputTextFlags_CharsHexadecimal);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGui::Text(
                        "%d", momiji::utils::sign_extend<std::int16_t>(ext16));
                }

                {
                    ImGui::Text("10 -> 16");
                    ImGui::SameLine();
                    static std::int32_t num10 = 0;
                    ImGui::PushItemWidth(70.0F);
                    ImGui::InputInt("##n10",
                                    &num10,
                                    0,
                                    0,
                                    ImGuiInputTextFlags_CharsDecimal);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGui::Text("%x", num10);
                }

                {
                    ImGui::Text("16 -> 10");
                    ImGui::SameLine();
                    static std::int32_t num16 = 0;
                    ImGui::PushItemWidth(70.0F);
                    ImGui::InputInt("##n16",
                                    &num16,
                                    0,
                                    0,
                                    ImGuiInputTextFlags_CharsHexadecimal);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGui::Text("%d", num16);
                }
            }
            ImGui::EndGroup();
            ImGui::End();
        }

        {
            ImGui::Begin("Settings");

            if (ImGui::BeginMenu("Theming"))
            {

                if (ImGui::MenuItem("Switch theme"))
                {
                    lightTheme = !lightTheme;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Toggle momiji's debug infos"))
                {
                    showDebugInfo = !showDebugInfo;
                }

                ImGui::EndMenu();
            }

            ImGui::End();
        }

        {
            ImGui::Begin("Emulator Settings",
                         nullptr,
                         ImGuiWindowFlags_AlwaysAutoResize);

            static std::array<std::string_view, 2> possibleSettingsStr { {
                "Always",
                "Never",
            } };

            ImGui::TextUnformatted("Retain states");

            if (ImGui::BeginCombo("##cb_states", emuSettings.toString().data()))
            {
                if (ImGui::Selectable(possibleSettingsStr[0].data()))
                {
                    emuSettings.retainStates =
                        momiji::EmulatorSettings::RetainStates::Always;
                    emu.loadNewSettings(emuSettings);
                }

                if (ImGui::Selectable(possibleSettingsStr[1].data()))
                {
                    emuSettings.retainStates =
                        momiji::EmulatorSettings::RetainStates::Never;
                    emu.loadNewSettings(emuSettings);
                }

                ImGui::EndCombo();
            }

            ImGui::TextUnformatted("Stack size (Bytes)");

            if (ImGui::InputInt("##in_size",
                                reinterpret_cast<int*>(&emuSettings.stackSize),
                                0))
            {
                emu.loadNewSettings(emuSettings);
            }

            ImGui::End();
        }

        {
            ImGui::Begin("Memory dump");

            const auto states = emu.constStates();

            if (states.size() > 1)
            {
                auto& lastSys = states.back();

                momiji::ConstExecutableMemoryView memview = lastSys.mem;
                const auto pc = lastSys.cpu.programCounter;

                const auto begin =
                    std::uint32_t(memview.executableMarker.begin);

                const auto end = std::uint32_t(memview.executableMarker.end);
                ;

                for (std::uint32_t i = begin; i < end; i += 2)
                {
                    momiji::DecodedInstruction instr =
                        momiji::decode(memview, i);

                    std::uint8_t higher = memview.read8(i).value_or(0);
                    std::uint8_t lower  = memview.read8(i + 1).value_or(0);

                    auto pcadd   = memview.begin() + pc.raw();
                    auto curradd = memview.begin() + i;

                    ImGui::TextUnformatted(pcadd == curradd ? "=>" : "  ");
                    ImGui::SameLine();
                    ImGui::Text("%.8x: %.2x %.2x %s",
                                i,
                                higher,
                                lower,
                                instr.string.c_str());
                }
            }
            else
            {
                ImGui::Text("No memory output available!\n");
            }

            ImGui::End();
        }

        {
            ImGui::Begin("Code view");

            ImGui::SameLine();
            if (ImGui::Button("Execute"))
            {
                emu.step();
            }

            ImGui::SameLine();
            if (ImGui::Button("Rollback"))
            {
                emu.rollback();
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset"))
            {
                emu.reset();
            }

            static std::string errorStr {};
            static std::optional<momiji::ParserError> err;

            ImGui::SameLine();

            if (ImGui::Button("Parse"))
            {
                err = emu.newState(str);
            }

            if (err.has_value())
            {
                momiji::ParserError& error = err.value();

                using namespace momiji::errors;

                if (errorStr.empty())
                {
                    // clang-format off
                    std::visit(asl::overloaded{
                        [&](const UnknownError& /* unused */) {
                            errorStr += "Unknown error.";
                        },

                        [&](const NoInstructionFound& par) {
                            errorStr += "instruction \"" + par.inputString +
                                        "\" not found.";

                            if (!par.alternatives.empty())
                            {
                                errorStr += "\nAlternatives:\n";
                                for (const auto& x : par.alternatives)
                                {
                                    errorStr += x + '\n';
                                }
                            }
                        },

                        [&](const NoLabelFound& par) {
                            errorStr += "label \"" + par.label +
                                        "\" not found.";
                        },

                        [&](const DataTypeMismatch& par) {
                            errorStr += "data type " +
                                        toString(par.inputDataType) +
                                        " is not valid for this instruction.";

                            if (!par.acceptedDataTypes.empty())
                            {
                                errorStr += "\nAccepted data types: ";
                                errorStr += contToString(par.acceptedDataTypes);
                            }
                        },

                        [&](const OperandTypeMismatch& par) {
                            errorStr += "operand type " + toString(par.inputOp)
                                        + " is not valid for this instruction.";

                            if (!par.acceptedOps.empty())
                            {
                                errorStr += "\nAccepted operands: ";
                                errorStr += contToString(par.acceptedOps);
                            }
                        },

                        [&](const InvalidRegisterNumber& par) {
                            errorStr += "Invalid register number " +
                                        std::to_string(par.input);
                        },

                        [&](const UnexpectedCharacter& par) {
                            errorStr += "Unexpected character '" +
                                        std::to_string(par.character) + "'.";
                        },

                        [&](const MissingCharacter& par) {
                            errorStr += "Missing a '" +
                                        std::string{par.character} + "'.";
                        },

                        [&](const UnknownOperand& /* unused */) {
                            errorStr += "Unknown operand, are you sure "
                                        "the syntax is valid?";
                        },

                        [&](const UnexpectedSectionContent& par) {
                            switch (par.section)
                            {
                            case momiji::ParserSection::Code:
                                errorStr += "Unexpected variable definition "
                                            "in code section";
                                break;

                            case momiji::ParserSection::Data:
                                errorStr += "Unexpected executable instruction "
                                            "in data section";
                                break;
                            }
                        }
                    }, error.errorType);

                    // clang-format on
                }

                ImGui::Text(
                    "Error at line %ld, %s", error.line, errorStr.data());
                ImGui::Text("Around: %.*s",
                            static_cast<int>(error.codeStr.size()),
                            error.codeStr.data());
            }
            else
            {
                // ImGui::Text("No errors!");
            }

            ImGui::BeginChild("Source code");
            ImGui::InputTextMultiline("", &str, ImGui::GetContentRegionAvail());
            ImGui::EndChild();

            ImGui::End();
        }

        {
            ImGui::Begin("Registers",
                         nullptr,
                         ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoResize);

            static bool hexDump = false;
            // static bool allowEditing = false;

            ImGui::Checkbox("Hex", &hexDump);
            // ImGui::Checkbox("Allow editing", &allowEditing);
            ImGui::Separator();
            ImGui::NewLine();

            auto& last = emu.states().back();

            ImGui::SameLine();
            ImGui::Text("Registers");

            ImGuiInputTextFlags flags = 0;
            if (hexDump)
            {
                flags |= ImGuiInputTextFlags_CharsHexadecimal;
            }

            flags |= ImGuiInputTextFlags_ReadOnly;

            ImGui::BeginGroup();
            ImGui::Text("Address");
            for (asl::isize i = 0; i < asl::ssize(last.cpu.addressRegisters);
                 ++i)
            {
                auto& reg = asl::saccess(last.cpu.addressRegisters, i);
                auto tmp  = reg.raw();
                ImGui::PushID(&reg);
                ImGui::Text("a%ld", i);
                ImGui::SameLine();
                ImGui::PushItemWidth(70.0F);
                ImGui::InputInt("##", &tmp, 0, 0, flags);
                ImGui::PopItemWidth();
                ImGui::PopID();
            }
            ImGui::EndGroup();

            ImGui::SameLine(ImGui::GetCursorPosX() + 100.0F);

            ImGui::BeginGroup();
            ImGui::Text("Data");
            for (asl::isize i = 0; i < asl::ssize(last.cpu.dataRegisters); ++i)
            {
                auto& reg = asl::saccess(last.cpu.dataRegisters, i);
                auto tmp  = reg.raw();
                ImGui::PushID(&reg);
                ImGui::Text("d%ld", i);
                ImGui::SameLine();
                ImGui::PushItemWidth(70.0F);
                ImGui::InputInt("##", &tmp, 0, 0, flags);
                ImGui::PopItemWidth();
                ImGui::PopID();
            }
            ImGui::EndGroup();

            auto pc = last.cpu.programCounter.ptr();
            ImGui::BeginGroup();
            ImGui::Text("PC");
            ImGui::SameLine();
            ImGui::PushItemWidth(70.0F);
            ImGui::InputInt(
                "##pc", reinterpret_cast<std::int32_t*>(pc), 0, 0, flags);

            if (!last.mem.empty())
            {
                const auto memview = momiji::make_memory_view(last);
                const auto pc      = last.cpu.programCounter;
                const auto addr =
                    reinterpret_cast<std::uint64_t>(memview.begin() + pc.raw());
                ImGui::SameLine();
                ImGui::Text("%lx", addr);
            }

            ImGui::PopItemWidth();
            ImGui::EndGroup();

            ImGui::Separator();

            ImGui::BeginGroup();

            ImGui::EndGroup();

            ImGui::End();
        }

        {
            ImGui::Begin("Stack");

            const auto states = emu.states();

            if (states.size() > 1)
            {
                const auto& lastSys = states.back();

                const momiji::ConstExecutableMemoryView memview = lastSys.mem;
                const auto sp = lastSys.cpu.addressRegisters[7];

                const auto maxStackLength =
                    memview.size() - emuSettings.stackSize;

                for (asl::isize i = asl::ssize(memview) - 2;
                     i >= maxStackLength;
                     i -= 2)
                {
                    std::uint8_t lower  = memview.read8(i).value_or(0);
                    std::uint8_t higher = 0;

                    if ((i - 1) >= maxStackLength)
                    {
                        higher = *memview.read8(i - 1);
                    }

                    auto pcadd   = memview.begin() + sp.raw();
                    auto curradd = memview.begin() + i;

                    ImGui::TextUnformatted(pcadd == curradd ? "=>" : "  ");
                    ImGui::SameLine();
                    ImGui::Text("%.8lx: %x %x", i, higher, lower);
                }
            }
            else
            {
                ImGui::Text("No stack output available!\n");
            }
            ImGui::End();
        }

        auto endtime = std::chrono::high_resolution_clock::now();
        auto as_millis =
            std::chrono::duration<double, std::milli>(endtime - begintime);

        if (as_millis <= fpsmax)
        {
            std::this_thread::sleep_for(fpsmax - as_millis);
        }

        MVP = proj;

        shader.enable();

        {
            int unif = shader.getUniformLocation("MVP");
            shader.setUniform(unif, MVP);
        }

        renderer.begin();
        renderer.add(background.getRenderable());
        renderer.end();

        renderer.draw();

        shader.disable();

        ImGui::Render();
        tewi::renderImGui(def_tag {}, win, ImGui::GetDrawData());
        win.swapBuffers();
        ImGui::EndFrame();
    }

    tewi::shutdownImGui(def_tag {});
}
