#shader vertex
#version 330 core

layout (location = 0) in vec4 modelPos;
layout (location = 1) in vec2 texturePos;
layout (location = 2) in float textureIdx;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_texturePos;
out float v_textureIdx;

void main()
{
    gl_Position = u_projection * u_view * u_model * modelPos;
    v_texturePos = texturePos;
    v_textureIdx = textureIdx;
};

#shader fragment
#version 330 core

in vec2 v_texturePos;
in float v_textureIdx;

layout(location = 0) out vec4 fragmentColor;

uniform sampler2D u_textureSamplerA;
uniform sampler2D u_textureSamplerB;

void main()
{
    fragmentColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    switch (int(v_textureIdx)) {
        case 0:
            fragmentColor = texture(u_textureSamplerA, v_texturePos);
            break;
        case 1:
            fragmentColor = texture(u_textureSamplerB, v_texturePos);
            break;
    }
};
