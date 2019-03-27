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

    bool hexDump = false;

    constexpr std::chrono::duration<double, std::milli> fpsmax{1000.0 / 60.0};

    std::string str;

    tewi::Sprite<def_tag> background{glm::vec3{0.0f, 0.0f, 0.0f}, "res/bgimage.png"};

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

            ImGui::InputText("Source code", &str);
            if (ImGui::Button("Parse"))
            {
                emu.parse(str);
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
            ImGui::Begin("Registers");

            ImGui::Checkbox("Hex", &hexDump);

            const auto& last = emu.getStates().back();

            ImGui::Text("Address registers");

            for (int i = 0; i < last.cpu.addressRegisters.size(); ++i)
            {
                if (hexDump)
                {
                    ImGui::Text("a%d: %x", i, last.cpu.addressRegisters[i].value);
                }
                else
                {
                    ImGui::Text("a%d: %d", i, last.cpu.addressRegisters[i].value);
                }
            }

            ImGui::Separator();

            ImGui::Text("Data registers");
            for (int i = 0; i < last.cpu.dataRegisters.size(); ++i)
            {
                if (hexDump)
                {
                    ImGui::Text("d%d: %x", i, last.cpu.dataRegisters[i].value);
                }
                else
                {
                    ImGui::Text("d%d: %d", i, last.cpu.dataRegisters[i].value);
                }
            }

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
