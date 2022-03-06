#ifndef BASIC_SQUARE_H_
#define BASIC_SQUARE_H_

#include <vector>

/**
 * The delta from the origin on the X axis for a tile.
 * Meaning, the width of a tile is this value mutliplied by 2.
 */
static constexpr const float tileDeltaX = 0.5F;
/**
 * The delta from the origin on the Y axis for a tile.
 * Meaning, the length of a tile is this value mutliplied by 2.
 */
static constexpr const float tileDeltaY = 0.5F;
/**
 * The depth on the Z axis for a tile.
 * Meaning, how far down/deep a tile goes underground.
 * Because each of our tile will always its top at 0 on Z axis.
 */
// static constexpr const float tileDepthZ = 0.2F;

static const std::vector<float> basicSquareVertices = {
    // clang-format off
    -tileDeltaX, -tileDeltaY,
    tileDeltaX, -tileDeltaY,
    tileDeltaX, tileDeltaY,

    tileDeltaX, tileDeltaY,
    -tileDeltaX, tileDeltaY,
    -tileDeltaX, -tileDeltaY
    // clang-format on
};

// Each vertex for a basic square is made of 2 float values
static const auto basicSquareVertexSize = 2;
static const auto basicSquareSizeOf = sizeof(float);

#endif