#pragma once

#include <memory>

#include <asl/types>
#include <tewi/Video/API/API.h>
#include <tewi/Video/IndexBuffer.h>
#include <tewi/Video/Renderable2D.hpp>
#include <tewi/Video/Shader.h>

#include <tewi/Platform/OpenGL/Glew.h>

namespace momiji
{
    template <typename APIType, asl::i16 MaxTextures>
    struct basic_renderer
    {
        void map();

        template <typename T>
        void add(const tewi::Renderable2D<T>& rend);

        void unmap();

        void draw();

        tewi::ShaderProgram<APIType> createShaderProgram();
    };

    template <asl::i16 MaxTextures = 64>
    struct basic_renderer<tewi::API::OpenGLTag>
    {
        basic_renderer();

        void map();

        template <typename T>
        void add(const tewi::Renderable2D<T>& rend);

        void unmap();

        void draw();

        tewi::ShaderProgram<tewi::API::OpenGLTag> createShaderProgram();

      private:
        GLuint m_VAO;
        GLuint m_VBO;
        std::unique_ptr<tewi::IndexBuffer<tewi::API::OpenGLTag>> m_IBO;

        const asl::i64 m_indicesSize = MaxTextures * 6;
        const asl::i64 m_bufferSize = sizeof(tewi::Vertex) * 4 * MaxTextures;
    };

    template <asl::i16 MaxTextures>
    basic_renderer<tewi::API::OpenGLTag>::basic_renderer()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_bufferSize, 0, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(tewi::Vertex),
                              (const void*)(0));

        // UV
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(tewi::Vertex),
                              (const void*)(offsetof(Vertex, uv)));

        // TID
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(tewi::Vertex),
                              (const void*)(offsetof(Vertex, textureID)));

        // Color
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                              sizeof(tewi::Vertex),
                              (const void*)(offsetof(Vertex, color)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::vector<GLuint> indices(m_indicesSize);
        for (asl::sizei i = 0, offset = 0; i < indices.size();
             i += 6, offset += 4)
        {
            indices[i] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
        }

        m_IBO = std::make_unique<IndexBuffer<API::OpenGLTag>>(indices);

        glBindVertexArray(0);
    }

    template <asl::i16 MaxTextures>
    void basic_renderer<tewi::API::OpenGLTag, MaxTextures>::map()
    {
        int textureUni = glGetUniformLocation("texture");
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    template <asl::i16 MaxTextures>
    template <typename T>
    void basic_renderer<tewi::API::OpenGLTag, MaxTextures>::add(
        const tewi::Renderable2D<T>& rend)
    {
    }

    template <asl::i16 MaxTextures>
    void basic_renderer<tewi::API::OpenGLTag, MaxTextures>::unmap()
    {
    }

    template <asl::i16 MaxTextures>
    void basic_renderer<tewi::API::OpenGLTag, MaxTextures>::draw()
    {
    }

} // namespace momiji
