#include "Gui.h"

#include <thread>
#include <chrono>

#include <tewi/Video/Window.hpp>
#include <tewi/Utils/ImGuiBindings.h>
#include <tewi/Input/InputBuffer.hpp>
#include <tewi/Video/Shader.hpp>
#include <tewi/Video/Sprite.h>
#include <tewi/Video/Renderer2D.hpp>
#include <tewi/Video/BatchRenderer2D.hpp>

#include "Emulator.h"

void gui()
{
    using def_tag = tewi::API::OpenGLTag;

    momiji::emulator emu;

    tewi::Window<def_tag> win{"momiji", tewi::Width{800}, tewi::Height{600}};

    tewi::InputBuffer inputBuffer;
    win.bindTo(inputBuffer);

    bool lightTheme = true;

    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    tewi::initImGui(def_tag{}, win);

    bool showDebugInfo = false;


    constexpr std::chrono::duration<double, std::milli> fpsmax{1000.0 / 120.0};

    std::string str;

    //tewi::Sprite<def_tag> background{glm::vec3{0.0f, 0.0f, 0.0f}, "res/bgimage.png"};

    while (!win.isClosed())
    {
        win.pollEvents(inputBuffer);
        tewi::newFrameImGui(def_tag{}, win);
        ImGui::NewFrame();

        auto begintime = std::chrono::high_resolution_clock::now();

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
            ImGui::Begin("Momiji's infos");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
            ImGui::Begin("Code");

            ImGui::InputTextMultiline("", &str);
            if (ImGui::Button("Parse"))
            {
                auto err = emu.parse(str);

                if (err.has_value())
                {
                    momiji::parser_error& error = err.value();
                    ImGui::Text("Error at %d:%d,", error.line, error.column);
                    switch (error.errorType)
                    {
                    case momiji::parser_error::error_type::NoInstructionFound:
                        ImGui::TextUnformatted("no instruction found.\n");
                        break;

                    case momiji::parser_error::error_type::UnexpectedCharacter:
                        ImGui::TextUnformatted("unexpected character.\n");
                        break;

                    case momiji::parser_error::error_type::WrongInstruction:
                        ImGui::TextUnformatted("no such instruction.\n");
                        break;

                    case momiji::parser_error::error_type::WrongOperandType:
                        ImGui::TextUnformatted("wrong operand type.\n");
                        break;
                    }
                }
            }

            if (ImGui::Button("Execute"))
            {
                emu.step();
            }

            if (ImGui::Button("Rollback"))
            {
                emu.rollback();
            }

            ImGui::End();
        }

        {
            ImGui::Begin("Registers", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

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

            ImGui::Separator();

            ImGui::End();
        }

        {
            ImGui::Begin("Stack");

            ImGui::End();
        }

        auto endtime = std::chrono::high_resolution_clock::now();
        auto as_millis = std::chrono::duration<double, std::milli>(endtime - begintime);

        if (as_millis <= fpsmax)
        {
            std::this_thread::sleep_for(fpsmax - as_millis);
        }


        ImGui::Render();
        win.clear();
        tewi::renderImGui(def_tag{}, win, ImGui::GetDrawData());
        win.swapBuffers();
    }

    tewi::shutdownImGui(def_tag{});
}
