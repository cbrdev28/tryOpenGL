#shader vertex
#version 410 core

layout (location = 0) in vec2 i_modelVertex;
layout (location = 1) in vec2 i_modelPosition;
layout (location = 2) in float i_modelRotationAngle;

mat4 zRotationMatrix(float angle) {
    float s = sin(angle);
    float c = cos(angle);

    return mat4(
          c,   s, 0.0, 0.0,
         -s,   c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 translationMatrix(vec3 dir) {
    return mat4(
          1.0,   0.0,   0.0, 0.0,
          0.0,   1.0,   0.0, 0.0,
          0.0,   0.0,   1.0, 0.0,
        dir.x, dir.y, dir.z, 1.0
    );
}

mat4 scaleMatrix(vec3 scale) {
    return mat4(
        scale.x,     0.0,     0.0, 0.0,
            0.0, scale.y,     0.0, 0.0,
            0.0,     0.0, scale.z, 0.0,
            0.0,     0.0,     0.0, 1.0
    );
}

uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_modelVertexPosition;

void main()
{
    v_modelVertexPosition = modelTransformation * vec4(i_modelVertex, 0.0, 1.0);
    mat4 modelTransformation = translationMatrix(vec3(i_modelPosition, 0.0)) * zRotationMatrix(i_modelRotationAngle);
    gl_Position = u_projection * u_view * modelTransformation * vec4(i_modelVertex, 0.0, 1.0);
};

#shader fragment
#version 410 core

in vec4 v_modelVertexPosition;

layout(location = 0) out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(v_modelVertexPosition.x, v_modelVertexPosition.y, 0.5, 1.0);
};
