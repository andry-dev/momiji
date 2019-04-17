#include "Renderer.h"

#include <tewi/Video/API/API.h>
#include <tewi/Video/Renderable2D.hpp>
#include <tewi/Video/Vertex.h>


namespace momiji
{
    constexpr auto g_maxSprites = 10;
    constexpr auto g_vertexSize = sizeof(tewi::Vertex);
    constexpr auto g_spriteSize = g_vertexSize * 4;
    constexpr auto g_bufferSize = g_spriteSize * g_maxSprites;
    constexpr auto g_indicesBufferSize = g_maxSprites * 6;

    constexpr asl::string_view vertshader = R"(
    #version 410 core

    layout (location = 0) in vec2 vertexPosition;
    layout (location = 1) in vec2 vertexUV;
    layout (location = 2) in float vertexTID;
    layout (location = 3) in vec4 vertexColor;

    uniform mat4 MVP;

    out vec2 fragPosition;
    out vec2 fragUV;
    out float fragTID;
    out vec4 fragColor;

    void main()
    {
        gl_Position = MVP * vec4(vertexPosition, 0.0, 1.0);

        fragPosition = vertexPosition;
        fragTID = vertexTID;
        fragUV = vec2(vertexUV.x, 1 - vertexUV.y);
        fragColor = vertexColor;
    }
    )";

    constexpr asl::string_view fragshader = R"(
    #version 410 core

    in vec2 fragPosition;
    in vec2 fragUV;
    in float fragTID;
    in vec4 fragColor;

    out vec4 color;

    uniform sampler2D texture2D;

    void main()
    {
        vec4 textureColor = texture(texture2D, fragUV);
        color = fragColor * textureColor;
    }
    )";

    renderer<tewi::API::OpenGLTag>::renderer()
        : m_IBO(0), m_VAO(0), m_VBO(0)
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, g_bufferSize, 0, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        // Pos
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(tewi::Vertex), (const void*)(0));

        // UV
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(tewi::Vertex), (const void*)(offsetof(tewi::Vertex, uv)));

        // TID
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(tewi::Vertex), (const void*)(offsetof(tewi::Vertex, textureID)));

        // Color
        glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(tewi::Vertex), (const void*)(offsetof(tewi::Vertex, color)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::vector<GLuint> indices(g_indicesBufferSize);
        for (asl::u32 i = 0, offset = 0; i < indices.size(); i += 6, offset += 4)
        {
            indices[  i  ] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
        }

        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    renderer<tewi::API::OpenGLTag>::~renderer()
    {
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_IBO);
    }

    void renderer<tewi::API::OpenGLTag>::begin()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBindVertexArray(m_VAO);
        m_buffer = reinterpret_cast<tewi::Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    }

    void renderer<tewi::API::OpenGLTag>::end()
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void renderer<tewi::API::OpenGLTag>::draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textures[1]);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

        glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_count = 0;
    }

    tewi::ShaderProgram<tewi::API::OpenGLTag>
    renderer<tewi::API::OpenGLTag>::createShaderProgram()
    {
        constexpr std::array<tewi::ShaderDescription, 2> shaders
        {
            tewi::ShaderDescription{ vertshader, tewi::ShaderType::Vertex },
            tewi::ShaderDescription{ fragshader, tewi::ShaderType::Fragment },
        };

        constexpr std::array<asl::string_view, 4> attribs
        {
            "vertexPosition",
            "vertexUV",
            "vertexTID",
            "vertexColor"
        };

        return tewi::ShaderProgram<tewi::API::OpenGLTag>({ shaders }, { attribs });
    }
}
