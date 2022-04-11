#shader vertex
#version 410 core

layout (location = 0) in vec2 i_modelVertex;
layout (location = 1) in vec2 i_texturePos;
layout (location = 2) in vec2 i_modelPos;
layout (location = 3) in vec2 i_modelScale;
layout (location = 4) in float i_modelAngle;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_texturePos;

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

void main()
{
    mat4 modelTransformation = translationMatrix(vec3(i_modelPos, 0.0))
                                * zRotationMatrix(i_modelAngle)
                                * scaleMatrix(vec3(i_modelScale, 1.0));
    gl_Position = u_projection * u_view * modelTransformation * vec4(i_modelVertex, 0.0, 1.0);
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