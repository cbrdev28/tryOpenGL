#ifndef BASIC_SQUARE_H_
#define BASIC_SQUARE_H_

#include <array>

/**
 * The delta from the origin on the X axis for a tile.
 * Meaning, the width of a tile is this value mutliplied by 2.
 */
const float tileDeltaX = 0.5F;
/**
 * The delta from the origin on the Y axis for a tile.
 * Meaning, the length of a tile is this value mutliplied by 2.
 */
const float tileDeltaY = 0.5F;
/**
 * The depth on the Z axis for a tile.
 * Meaning, how far down/deep a tile goes underground.
 * Because each of our tile will always its top at 0 on Z axis.
 */
// static constexpr const float tileDepthZ = 0.2F;

const std::array<float, 12> basicSquareVertices = {
    // clang-format off
    -tileDeltaX, -tileDeltaY, // Bottom left
    tileDeltaX, -tileDeltaY, // Bottom right
    tileDeltaX, tileDeltaY, // Top right

    tileDeltaX, tileDeltaY, // Top right
    -tileDeltaX, tileDeltaY, // Top left
    -tileDeltaX, -tileDeltaY // Bottom left
    // clang-format on
};

const std::array<float, 8> basicSquareIndicedVertices = {
    // clang-format off
    -tileDeltaX, -tileDeltaY, // Bottom left: indice = 0
    tileDeltaX, -tileDeltaY, // Bottom right: indice = 1
    tileDeltaX, tileDeltaY, // Top right: indice = 2
    -tileDeltaX, tileDeltaY // Top left: indice = 3
    // clang-format on
};

const std::array<unsigned int, 6> basicSquareIndices = {
    // clang-format off
    0, 1, 2,
    2, 3, 0
    // clang-format on
};

const auto basicSquareVertexSize = 2;  // Each vertex for a basic square is made of 2 float values
const auto basicSquareVerticesSizeOf = sizeof(float);
const auto basicSquareIndicesSizeOf = sizeof(unsigned int);

#endif