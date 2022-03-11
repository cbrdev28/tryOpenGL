#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 posVec;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    posVec = aPos;
};

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 posVec;

void main()
{
    FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * vec4(posVec, 1.0f);
};
