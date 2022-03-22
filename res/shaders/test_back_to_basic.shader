#shader vertex
#version 330 core

layout (location = 0) in vec2 modelVertex;
layout (location = 1) in mat4 modelTransformation;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_modelVertexPosition;

void main()
{
    vec4 transformedModel = modelTransformation * vec4(modelVertex, 0.0, 1.0);
    gl_Position = u_projection * u_view * transformedModel;
    v_modelVertexPosition = transformedModel;
};

#shader fragment
#version 330 core

in vec4 v_modelVertexPosition;

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = v_modelVertexPosition;
};
