#ifndef DYNAMIC_TRIANGLE_H_
#define DYNAMIC_TRIANGLE_H_

#include <array>
#include <random>
#include <vector>

#include "../helpers/openGLErrorHelpers.h"

struct DynamicTriangle {
  static constexpr float kTriangleSize = 0.2F;

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

  inline static auto makeTriangleVertices(unsigned int gridRowColumnCount, float gridTileSize, float gridTileSpacing)
      -> std::vector<float> {
    std::default_random_engine gen{std::random_device{}()};
    const float gridRightEdgePos = (static_cast<float>(gridRowColumnCount) / 2.0F) * (gridTileSize + gridTileSpacing);
    const float gridTopPos = gridRightEdgePos;

    std::uniform_real_distribution<float> dist(0.0F, 1.0F);
    const float randomPosX = (dist(gen) * gridRightEdgePos * 2) - gridRightEdgePos;
    const float randomPosY = (dist(gen) * gridTopPos * 2) - gridTopPos;

    return std::vector<float>{
        // clang-format off
    randomPosX, randomPosY, // Bottom left: indice = 0
    randomPosX + kTriangleSize, randomPosY, // Bottom right: indice = 1
    randomPosX, randomPosY + kTriangleSize, // Top right: indice = 2
        // clang-format on
    };
  }
};

#endif