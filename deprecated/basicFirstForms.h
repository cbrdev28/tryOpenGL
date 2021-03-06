#ifndef BASIC_FIRST_FORMS_H_
#define BASIC_FIRST_FORMS_H_

#include <array>

#include "../common/glmHeaders.h"
#include "../common/openGLHeaders.h"

const GLfloat basicBackgroundNeonPinkR = 255.0F / 255.0F;
const GLfloat basicBackgroundNeonPinkG = 68.0F / 255.0F;
const GLfloat basicBackgroundNeonPinkB = 205.0F / 255.0F;

/**
 * How far the camera will look from the Z axis.
 * Meaning, "how high" the camera will be.
 * Used with the view matrix.
 */
const GLfloat basicCameraZDelta = 2.0F;
/**
 * How far the camera will look from the Y axis.
 * Meaning, "how behind" the camera will be.
 * Used with the view matrix.
 */
const GLfloat basicCameraYDelta = 2.0F;
/**
 * How fast the camera will move
 */
const double basicCameraSpeed = 1.0F;

// Pre-defined vector as helper when moving camera forward/backward
const auto basicCameraYAxis = glm::vec3(0.0F, 1.0F, 0.0F);

// By default, we look at things from higher (Z axis) and from behind (Y axis)
// Each time we apply this offset when setting the camera position
const glm::vec3 basicCameraPositionOffset = glm::vec3(0.0F, -basicCameraYDelta, basicCameraZDelta);
// The position of where the camera looks at (its target), by default the origin-ish.
const glm::vec3 basicCameraTarget = glm::vec3(0.0F, 0.0F, -1.0F);
// Define a vector which points up
const glm::vec3 basicCameraUp = glm::vec3(0.0F, 1.0F, 0.0F);

/**
 * The delta from the origin on the X axis for a tile.
 * Meaning, the width of a tile is this value mutliplied by 2.
 */
const GLfloat tileDeltaX = 0.5F;
/**
 * The delta from the origin on the Y axis for a tile.
 * Meaning, the length of a tile is this value mutliplied by 2.
 */
const GLfloat tileDeltaY = 0.5F;
/**
 * The depth on the Z axis for a tile.
 * Meaning, how far down/deep a tile goes underground.
 * Because each of our tile will always its top at 0 on Z axis.
 */
// static constexpr const GLfloat tileDepthZ = 0.2F;

const std::array<GLfloat, 12> basicSquareVertices = {
    // clang-format off
    -tileDeltaX, -tileDeltaY, // Bottom left
    tileDeltaX, -tileDeltaY, // Bottom right
    tileDeltaX, tileDeltaY, // Top right

    tileDeltaX, tileDeltaY, // Top right
    -tileDeltaX, tileDeltaY, // Top left
    -tileDeltaX, -tileDeltaY // Bottom left
    // clang-format on
};

const std::array<GLfloat, 8> basicSquareIndicedVertices = {
    // clang-format off
    -tileDeltaX, -tileDeltaY, // Bottom left: indice = 0
    tileDeltaX, -tileDeltaY, // Bottom right: indice = 1
    tileDeltaX, tileDeltaY, // Top right: indice = 2
    -tileDeltaX, tileDeltaY // Top left: indice = 3
    // clang-format on
};

const std::array<GLuint, 6> basicSquareIndices = {
    // clang-format off
    0, 1, 2,
    2, 3, 0
    // clang-format on
};

const auto basicSquareVertexSize = 2;  // Each vertex for a basic square is made of 2 GLfloat values
const auto basicSquareVerticesSizeOf = sizeof(GLfloat);
const auto basicSquareIndicesSizeOf = sizeof(GLuint);

const GLfloat triangleDeltaX = 0.2F;
const GLfloat triangleDeltaY = 0.3F;

const std::array<GLfloat, 6> basicTriangleIndicedVertices = {
    // clang-format off
    -triangleDeltaX, -triangleDeltaY, // Bottom left: indice = 0
    triangleDeltaX, -triangleDeltaY, // Bottom right: indice = 1
    triangleDeltaX - triangleDeltaX, triangleDeltaY // Top right: indice = 2
    // clang-format on
};

const std::array<GLuint, 3> basicTriangleIndices = {
    // clang-format off
    0, 1, 2
    // clang-format on
};

const auto basicTriangleVertexSize = 2;  // Each vertex for a basic triangle is made of 2 GLfloat values
const auto basicTriangleVerticesSizeOf = sizeof(GLfloat);
const auto basicTriangleIndicesSizeOf = sizeof(GLuint);

#endif