#shader vertex
#version 330 core

layout (location = 0) in vec4 modelPos;
layout (location = 1) in vec2 texturePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_texturePos;

void main()
{
    gl_Position = projection * view * model * modelPos;
    v_texturePos = texturePos;
};

#shader fragment
#version 330 core

in vec2 v_texturePos;

layout(location = 0) out vec4 fragmentColor;

uniform sampler2D textureSampler;

void main()
{
    vec4 textureColor = texture(textureSampler, v_texturePos);
    fragmentColor = textureColor;
};
