#ifndef BASIC_TRIANGLE_H_
#define BASIC_TRIANGLE_H_

#include <vector>

const float triangleDeltaX = 0.2F;
const float triangleDeltaY = 0.3F;

const std::vector<float> basicTriangleIndicedVertices = {
    // clang-format off
    -triangleDeltaX, -triangleDeltaY, // Bottom left: indice = 0
    triangleDeltaX, -triangleDeltaY, // Bottom right: indice = 1
    triangleDeltaX - triangleDeltaX, triangleDeltaY, // Top right: indice = 2
    // clang-format on
};

const std::vector<unsigned int> basicTriangleIndices = {
    // clang-format off
    0, 1, 2,
    // clang-format on
};

const auto basicTriangleVertexSize = 2;  // Each vertex for a basic triangle is made of 2 float values
const auto basicTriangleVerticesSizeOf = sizeof(float);
const auto basicTriangleIndicesSizeOf = sizeof(unsigned int);

#endif