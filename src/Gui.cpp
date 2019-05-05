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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tewi/Utils/TickTimer.h>

#include "Emulator.h"
#include "Renderer.h"

#include <cstdio>

static void MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    std::fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
  TEWI_EXPECTS(0, "");
}

void gui()
{
    using def_tag = tewi::API::OpenGLTag;

    momiji::Emulator emu;

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

    tewi::Sprite<def_tag> background{glm::vec2{0.0f, 0.0f}, "res/bgimage.png"};

    using BatchRenderer2D = tewi::Renderer2D<def_tag, tewi::BatchRenderer2D>;
    momiji::renderer<def_tag> renderer{};

    auto shader = renderer.createShaderProgram();

    auto proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    glm::mat4 MVP = proj;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    std::string error_string = "";

    while (!win.isClosed())
    {
        auto begintime = std::chrono::high_resolution_clock::now();

        win.clear();

        tewi::newFrameImGui(def_tag{}, win);
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
                    momiji::ParserError& error = err.value();
                    ImGui::Text("Error at %d:%d,", error.line, error.column);
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
                    }
                }
                else
                {
                    error_string = "";
                }
            }

            ImGui::TextUnformatted(error_string.c_str());

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
        tewi::renderImGui(def_tag{}, win, ImGui::GetDrawData());
        win.swapBuffers();
        ImGui::EndFrame();

    }


    tewi::shutdownImGui(def_tag{});
}
