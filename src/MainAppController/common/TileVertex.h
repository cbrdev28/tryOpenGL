#ifndef TILE_VERTEX_H
#define TILE_VERTEX_H

#include <array>
#include <vector>

#include "openGLErrorHelpers.h"

struct TileVertex {
  static constexpr unsigned int kPosCount = 2;
  static constexpr unsigned int kTextureCoordCount = 2;
  static constexpr unsigned int kTextureIdCount = 1;
  static constexpr float kTileSize = 1.0F;
  static constexpr float kTileSpacing = 0.01F;
  static constexpr unsigned int kTileVerticesCount = 4;

  std::array<float, kPosCount> positions;
  std::array<float, kTextureCoordCount> textureCoords;
  float textureIdx;

  // Build a grid of N tiles per row & column, centered around 0x:0Y coordinates
  // All tiles will have texture ID set to 0 (to draw grass),except on the edge of the grid,
  // tiles will be set to texture ID 1 (to draw wall)
  // NOTE: verticesCount must be equal to: rowColumnSize * rowColumnSize * kTileVerticesCount
  template <unsigned int verticesCount>
  inline static auto buildTilesGrid(unsigned int rowColumnSize) -> std::array<TileVertex, verticesCount> {
    ASSERT(verticesCount <= 2048);  // Make sure we're not building a crazy grid!
    // NOTE: check for overflow?
    ASSERT(verticesCount == rowColumnSize * rowColumnSize * kTileVerticesCount);

    std::array<TileVertex, verticesCount> vertices;

    for (unsigned int i = 0; i < rowColumnSize; i += 1) {
      for (unsigned int j = 0; j < rowColumnSize; j += 1) {
        auto posX = static_cast<float>(j) * (kTileSize + kTileSpacing);
        posX = posX - (static_cast<float>(rowColumnSize) / 2.0F);  // To center the grid of tiles

        auto posY = static_cast<float>(i) * (kTileSize + kTileSpacing);
        posY = posY - (static_cast<float>(rowColumnSize) / 2.0F);  // To center the grid of tiles

        float tileTextureId = 0.0F;
        if (j == 0 || j == rowColumnSize - 1 || i == 0 || i == rowColumnSize - 1) {
          // Draw "wall" texture on the edge of the grid
          tileTextureId = 1.0F;
        }

        TileVertex vertex1 = {{posX, posY}, {0.0F, 0.0F}, tileTextureId};
        TileVertex vertex2 = {{posX + kTileSize, posY}, {1.0F, 0.0F}, tileTextureId};
        TileVertex vertex3 = {{posX + kTileSize, posY + kTileSize}, {1.0F, 1.0F}, tileTextureId};
        TileVertex vertex4 = {{posX, posY + kTileSize}, {0.0F, 1.0F}, tileTextureId};

        const auto offset = i * rowColumnSize * kTileVerticesCount;
        ASSERT(j + 0 + offset < verticesCount);
        ASSERT(j + 1 + offset < verticesCount);
        ASSERT(j + 2 + offset < verticesCount);
        ASSERT(j + 3 + offset < verticesCount);
        vertices.at((j * 4) + 0 + offset) = vertex1;
        vertices.at((j * 4) + 1 + offset) = vertex2;
        vertices.at((j * 4) + 2 + offset) = vertex3;
        vertices.at((j * 4) + 3 + offset) = vertex4;
      }
    }
    return vertices;
  }

  template <unsigned int verticesCount>
  inline static auto serialize(const std::array<TileVertex, verticesCount>& tileVertices) -> std::vector<float> {
    // The goal is to transform a vector of TileVertex into a vector of float
    // Count how many floats we have in 1 TileVertex (used to reserve vector size)
    const auto floatValuesCount = kPosCount + kTextureCoordCount + kTextureIdCount;
    // TODO(cbr): try to change this into an array?
    std::vector<float> serializedTiles = {};
    serializedTiles.reserve(floatValuesCount * tileVertices.size());

    for (auto& tileVertex : tileVertices) {
      serializedTiles.emplace_back(tileVertex.positions[0]);
      serializedTiles.emplace_back(tileVertex.positions[1]);
      serializedTiles.emplace_back(tileVertex.textureCoords[0]);
      serializedTiles.emplace_back(tileVertex.textureCoords[1]);
      serializedTiles.emplace_back(tileVertex.textureIdx);
    }
    return serializedTiles;
  }
};

#endif