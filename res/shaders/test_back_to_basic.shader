#shader vertex
#version 330 core

layout (location = 0) in vec2 modelVertex;
layout (location = 1) in vec2 modelPosition;
layout (location = 2) in mat4 modelTransformation;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_modelVertexPosition;

void main()
{
    vec2 modelVertexPosition = modelVertex + modelPosition;
    gl_Position = u_projection * u_view * u_model * modelTransformation * vec4(modelVertexPosition, 0.0, 1.0);
    v_modelVertexPosition = modelVertexPosition;
};

#shader fragment
#version 330 core

in vec2 v_modelVertexPosition;

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(v_modelVertexPosition, 0.5, 1.0);
};
