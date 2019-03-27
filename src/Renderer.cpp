#pragma once

constexpr auto vertshader = R"(
#version 410 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in float vertexTID;
layout (location = 3) in vec4 vertexColor;

out vec2 fragPosition;
out vec2 fragUV;
out float fragTID;
out vec4 fragColor;

uniform mat4 MVP;

void main()
{
    gl_Position = (MVP * vec4(vertexPosition, 0.0, 1.0));

    fragPosition = vertexPosition;
    fragUV = vec2(vertexUV.x, 1 - vertexUV.y);
    fragTID = vertexTID;
    fragColor = vertexColor;
}
)";

constexpr auto fragshader = R"(
#version 410 core

in vec2 fragPosition;
in vec2 fragUV;
in float fragTID;
in vec4 fragColor;

out vec4 color;

uniform sampler2D texture;

void main()
{
    int tid = fragTID;
    vec4 textureColor = texture(texture, fragUV);
    color = fragColor * textureColor;
}
)";

namespace momiji
{
}
