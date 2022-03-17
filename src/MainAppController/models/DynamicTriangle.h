#ifndef DYNAMIC_TRIANGLE_H_
#define DYNAMIC_TRIANGLE_H_

#include <array>

#include "openGLErrorHelpers.h"

struct DynamicTriangle {
  template <unsigned int indicesCount>
  inline static auto makeIndices() -> std::array<unsigned int, indicesCount> {
    // Make indices to draw basic triangle (3 indices for each triangle)
    std::array<unsigned int, indicesCount> indices;
    for (int i = 0; i < indicesCount; i += 3) {
      ASSERT(i + 2 < indicesCount);
      indices.at(i + 0) = 0 + i;
      indices.at(i + 1) = 1 + i;
      indices.at(i + 2) = 2 + i;
    }
    return indices;
  }
};

#endif