#shader vertex
#version 330 core

layout (location = 0) in vec4 modelPosition;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_modelPosition;

void main()
{
    gl_Position = u_projection * u_view * u_model * modelPosition;
    v_modelPosition = modelPosition;
};

#shader fragment
#version 330 core

out vec4 fragmentColor;

in vec4 v_modelPosition;

void main()
{
    fragmentColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) + v_modelPosition;
};
