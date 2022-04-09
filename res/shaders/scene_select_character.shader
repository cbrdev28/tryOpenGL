#shader vertex
#version 410 core

layout (location = 0) in vec2 i_modelVertex;
layout (location = 1) in vec2 i_texturePos;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_texturePos;

void main()
{
    gl_Position = u_projection * u_view * vec4(i_modelVertex, 0.0, 1.0);
    v_texturePos = i_texturePos;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 fragmentColor;

uniform sampler2D u_textureSampler;

in vec2 v_texturePos;

void main()
{
    vec4 fragmentTexture = texture(u_textureSampler, v_texturePos);
    fragmentColor = fragmentTexture;
    // fragmentColor = vec4(0.6, 0.5, 0.4, 1.0);
}
