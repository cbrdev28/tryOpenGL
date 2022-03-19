#shader vertex
#version 330 core

layout (location = 0) in vec2 modelPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_modelPos;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(modelPos, 0.0, 1.0);
    v_modelPos = modelPos;
};

#shader fragment
#version 330 core

in vec2 v_modelPos;

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(v_modelPos, 0.0, 1.0);
};
