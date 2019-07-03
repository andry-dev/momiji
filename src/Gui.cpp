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

#include "Emulator.h"
#include "Renderer.h"
#include <Utils.h>

#include <cstdio>

void gui()
{
    using def_tag = tewi::API::OpenGLTag;

    momiji::EmulatorSettings emuSettings;
    emuSettings.programStart = 0;
    emuSettings.dataSectionOffset = 0;
    emuSettings.retainStates = momiji::EmulatorSettings::RetainStates::Always;
    //emuSettings.parserSettings.breakpoints = gsl::null_span{};

    momiji::Emulator emu{emuSettings};

    tewi::Window<def_tag> win { "momiji", tewi::Width { 800 },
                                tewi::Height { 600 } };

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

    tewi::Sprite<def_tag> background { glm::vec2 { 0.0f, 0.0f },
                                       "res/bgimage.png" };

    using BatchRenderer2D = tewi::Renderer2D<def_tag, tewi::BatchRenderer2D>;
    momiji::renderer<def_tag> renderer {};

    auto shader = renderer.createShaderProgram();

    auto proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
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
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);

            ImGui::Separator();

            ImGui::BeginGroup();
            {
                {
                    ImGui::Text("sign_extend<i8>");
                    ImGui::SameLine();
                    static std::int32_t ext8 = 0;
                    ImGui::PushItemWidth(70.0f);
                    ImGui::InputInt("##1", &ext8, 0, 0,
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
                    ImGui::PushItemWidth(70.0f);
                    ImGui::InputInt("##2", &ext16, 0, 0,
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
                    ImGui::PushItemWidth(70.0f);
                    ImGui::InputInt("##n10", &num10, 0, 0,
                                    ImGuiInputTextFlags_CharsDecimal);
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGui::Text("%x", num10);
                }

                {
                    ImGui::Text("16 -> 10");
                    ImGui::SameLine();
                    static std::int32_t num16 = 0;
                    ImGui::PushItemWidth(70.0f);
                    ImGui::InputInt("##n16", &num16, 0, 0,
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
            ImGui::Begin("Emulator Settings");

            static std::array<std::string_view, 3> possibleSettingsStr {{
                "Always",
                "Never",
            }};

            ImGui::TextUnformatted("Retain states");

            if (ImGui::BeginCombo("", emuSettings.toString().data()))
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

            ImGui::End();
        }

        {
            ImGui::Begin("Memory dump");

            const auto& states = emu.getStates();

            if (states.size() > 1)
            {
                const auto& lastSys = states.back();

                auto memview = momiji::make_memory_view(lastSys);
                const auto pc = lastSys.cpu.programCounter.address;

                for (int i = 0; i < memview.size(); ++i)
                {
                    std::uint8_t lower = memview[i] & 0x00FF;
                    std::uint8_t higher = (memview[i] & 0xFF00) >> 8;

                    auto pcadd = memview.begin() + pc;
                    auto curradd = memview.begin() + i;

                    ImGui::TextUnformatted(pcadd == curradd ? "=>" : "  ");
                    ImGui::SameLine();
                    ImGui::Text("%x: %x %x", &memview[i], higher, lower);
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
                emu.rollbackSys();
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset"))
            {
                emu.reset();
            }

            static std::string error_string = "";
            static std::optional<momiji::ParserError> err;

            ImGui::SameLine();

            if (ImGui::Button("Parse"))
            {
                err = emu.newState(str);
            }

            if (err.has_value())
            {
                momiji::ParserError& error = err.value();
                switch (error.errorType)
                {
                case momiji::ParserError::ErrorType::NoInstructionFound:
                    error_string = "no instruction found.";
                    break;

                case momiji::ParserError::ErrorType::UnexpectedCharacter:
                    error_string = "unexpected character.";
                    break;

                case momiji::ParserError::ErrorType::WrongInstruction:
                    error_string = "no such instruction.";
                    break;

                case momiji::ParserError::ErrorType::WrongOperandType:
                    error_string = "wrong operand type.";
                    break;

                case momiji::ParserError::ErrorType::NoLabelFound:
                    error_string = "no label found.";
                    break;

                case momiji::ParserError::ErrorType::WrongRegisterNumber:
                    error_string = "wrong register number.";
                    break;

                default:
                    error_string = "unknown error.";
                    break;
                }
                ImGui::Text("Error at line %d, %s", error.line, error_string.data());
            }
            else
            {
                //ImGui::Text("No errors!");
            }

            ImGui::BeginChild("Source code");
            ImGui::InputTextMultiline("", &str, ImGui::GetContentRegionAvail());
            ImGui::EndChild();

            ImGui::End();
        }

        {
            ImGui::Begin("Registers", 0,
                         ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoResize);

            static bool hexDump = false;
            static bool allowEditing = false;

            ImGui::Checkbox("Hex", &hexDump);
            ImGui::Checkbox("Allow editing", &allowEditing);
            ImGui::Separator();
            ImGui::NewLine();

            const auto& last = emu.getStates().back();

            ImGui::SameLine();
            ImGui::Text("Registers");

            ImGuiInputTextFlags flags = 0;
            if (hexDump)
            {
                flags |= ImGuiInputTextFlags_CharsHexadecimal;
            }

            if (!allowEditing)
            {
                flags |= ImGuiInputTextFlags_ReadOnly;
            }

            ImGui::BeginGroup();
            ImGui::Text("Address");
            for (int i = 0; i < last.cpu.addressRegisters.size(); ++i)
            {
                auto& reg = last.cpu.addressRegisters[i];
                ImGui::PushID(&reg);
                ImGui::Text("a%d", i);
                ImGui::SameLine();
                ImGui::PushItemWidth(70.0f);
                ImGui::InputInt("##", (int*)&reg.value, 0, 0, flags);
                ImGui::PopItemWidth();
                ImGui::PopID();
            }
            ImGui::EndGroup();

            ImGui::SameLine(ImGui::GetCursorPosX() + 100.0f);

            ImGui::BeginGroup();
            ImGui::Text("Data");
            for (int i = 0; i < last.cpu.dataRegisters.size(); ++i)
            {
                auto& reg = last.cpu.dataRegisters[i];
                ImGui::PushID(&reg);
                ImGui::Text("d%d", i);
                ImGui::SameLine();
                ImGui::PushItemWidth(70.0f);
                ImGui::InputInt("##", (int*)&reg.value, 0, 0, flags);
                ImGui::PopItemWidth();
                ImGui::PopID();
            }
            ImGui::EndGroup();

            ImGui::BeginGroup();
            ImGui::Text("PC");
            ImGui::SameLine();
            ImGui::PushItemWidth(70.0f);
            ImGui::InputInt("##pc", (int*)&last.cpu.programCounter.address, 0,
                            0, flags);
            if (!last.mem.empty())
            {
                const auto memview = momiji::make_memory_view(last);
                const auto pc = last.cpu.programCounter.address;
                const auto addr = reinterpret_cast<std::uint64_t>(memview.begin() + pc);
                ImGui::SameLine();
                ImGui::Text("%x", addr);
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
