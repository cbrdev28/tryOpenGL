#shader vertex
#version 410 core

layout (location = 0) in vec2 i_modelVertex;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * vec4(i_modelVertex, 0.0, 1.0);
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(0.6, 0.5, 0.4, 1.0);
}
