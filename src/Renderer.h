#pragma once

#include <tewi/Platform/OpenGL/Glew.h>

#include <array>

#include <tewi/Video/Vertex.h>
#include <tewi/Video/Shader.hpp>

namespace tewi
{
    namespace API
    {
        struct OpenGLTag;
    }

    struct Vertex;
}

namespace momiji
{
    template <typename APIType>
    struct renderer
    { };

    template <>
    struct renderer<tewi::API::OpenGLTag>
    {
        renderer();
        ~renderer();

        void begin();

        template <typename Renderable>
        void add(const Renderable& rend);
        void end();
        void draw();

        static tewi::ShaderProgram<tewi::API::OpenGLTag> createShaderProgram();

    private:
        GLuint m_IBO{0};
        GLuint m_VAO{0};
        GLuint m_VBO{0};

        int m_count{0};

        tewi::Vertex* m_buffer{nullptr};
        std::array<int, 2> m_textures{ 0, 0 };
    };

    template <typename Renderable>
    void renderer<tewi::API::OpenGLTag>::add(const Renderable& rend)
    {
        const auto& size = rend.texture.size;
        const auto tid = rend.texture.id;
        const auto scale = rend.scale;

        for (auto& saved_texture : m_textures)
        {
            if (tid == saved_texture)
            {
                continue;
            }

            if (saved_texture == 0 && tid != saved_texture)
            {
                saved_texture = tid;
            }
        }

        m_buffer->position = glm::vec2(rend.pos.x, rend.pos.y + size.y * scale);
        m_buffer->color = rend.color;
        m_buffer->uv = glm::vec2(0.0f, 1.0f);
        m_buffer->textureID = tid;
        m_buffer++;

        m_buffer->position = glm::vec2(rend.pos.x + size.x * scale, rend.pos.y + size.y * scale);
        m_buffer->color = rend.color;
        m_buffer->uv = glm::vec2(1.0f, 1.0f);
        m_buffer->textureID = tid;
        m_buffer++;

        m_buffer->position = glm::vec2(rend.pos.x + size.x * scale, rend.pos.y);
        m_buffer->color = rend.color;
        m_buffer->uv = glm::vec2(1.0f, 0.0f);
        m_buffer->textureID = tid;
        m_buffer++;

        m_buffer->position = rend.pos;
        m_buffer->color = rend.color;
        m_buffer->uv = glm::vec2(0.0f, 0.0f);
        m_buffer->textureID = tid;
        m_buffer++;

        m_count += 6;
    }
}
