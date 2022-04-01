#shader vertex
#version 330 core

layout (location = 0) in vec4 modelPos;
layout (location = 1) in vec2 texturePos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_texturePos;

void main()
{
    gl_Position = u_projection * u_view * u_model * modelPos;
    v_texturePos = texturePos;
};

#shader fragment
#version 330 core

in vec2 v_texturePos;

layout(location = 0) out vec4 fragmentColor;

uniform sampler2D u_textureSampler;

void main()
{
    vec4 textureFragment = texture(u_textureSampler, v_texturePos);
    fragmentColor = textureFragment;
};
