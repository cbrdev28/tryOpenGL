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
  static constexpr std::size_t kTileFloatValuesCount = kPosCount + kTextureCoordCount + kTextureIdCount;

  std::array<float, kPosCount> positions;
  std::array<float, kTextureCoordCount> textureCoords;
  float textureIdx;

  // Build a grid of N tiles per row & column, centered around 0x:0Y coordinates
  // All tiles will have texture ID set to 0 (to draw grass),except on the edge of the grid,
  // tiles will be set to texture ID 1 (to draw wall)
  // NOTE: verticesCount must be equal to: rowColumnSize * rowColumnSize * kTileVerticesCount
  template <unsigned int verticesCount>
  inline static auto buildTilesGrid(unsigned int rowColumnSize) -> std::array<TileVertex, verticesCount> {
    // Make sure we're not building a crazy grid!
    ASSERT(verticesCount <= 2048);
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

  // The goal is to transform an array of TileVertex into an array of float
  template <unsigned int verticesCount>
  inline static auto serialize(const std::array<TileVertex, verticesCount>& gridVertices)
      -> std::array<float, kTileFloatValuesCount * verticesCount> {
    // Total of float values needed for the grid
    const auto floatVerticesCount = kTileFloatValuesCount * verticesCount;
    std::array<float, floatVerticesCount> serializedVertices;

    for (int i = 0; i < gridVertices.size(); i++) {
      serializedVertices.at(i * kTileFloatValuesCount + 0) = gridVertices.at(i).positions[0];
      serializedVertices.at(i * kTileFloatValuesCount + 1) = gridVertices.at(i).positions[1];
      serializedVertices.at(i * kTileFloatValuesCount + 2) = gridVertices.at(i).textureCoords[0];
      serializedVertices.at(i * kTileFloatValuesCount + 3) = gridVertices.at(i).textureCoords[1];
      serializedVertices.at(i * kTileFloatValuesCount + 4) = gridVertices.at(i).textureIdx;
    }
    return serializedVertices;
  }

  // Given a coordinate & a grid, return the index (of the first vertext) of the tile matching in the grid
  // The grid size & grid row/column size are needed for verification
  template <unsigned int verticesCount>
  inline static auto findTileBaseIdxForPos(float posX, float posY,
                                           const std::array<TileVertex, verticesCount>& vertices, unsigned int gridSize,
                                           unsigned int rowColumnSize) -> int {
    const unsigned int tilesCount = vertices.size() / kTileVerticesCount;
    ASSERT(tilesCount == gridSize);

    unsigned int maxCount = tilesCount;
    unsigned int startCount = 0;
    // We only try to optimize if there is a "large" amount of tiles
    const bool optimizeThreshold = tilesCount > 16;

    if (optimizeThreshold) {
      // We know/hope our grid is centered at coordinate 0/0
      // So if the position of the camera is negative on Y axis,
      // it means we "should" only search in the bottom rows of our tile grid
      if (posY < 0) {
        // Search only for half the count (plus the number for one more row/column)
        maxCount = (tilesCount / 2) + rowColumnSize;
      } else if (posY >= 0) {
        // Skip first half (minus the number for one row/column)
        startCount = (tilesCount / 2) - rowColumnSize;
      }
    }
    ASSERT(startCount >= 0);
    ASSERT(maxCount <= tilesCount);

    for (unsigned int i = startCount; i < maxCount; i++) {
      if (!optimizeThreshold) {
        continue;
      } else {
        const auto idxLine = i % rowColumnSize;
        if (posX < 0.0F) {
          // Skip tiles at the end of lines
          // If we passed at least half of the line
          if (idxLine > rowColumnSize / 2) {
            // Bump current "i" index to finish the line
            const auto offset = rowColumnSize - idxLine - 1;
            i = i + offset;
          }
        } else if (posX >= 0.0F) {
          // Skip tiles from the beginning of lines
          // If we are still at least in the first half
          if (idxLine < rowColumnSize / 2) {
            // Bump current "i" index to the middle of line
            const auto offset = (rowColumnSize / 2) - 1;
            i = i + offset;
          }
        }
      }
      const auto vertex1Idx = i * kTileVerticesCount + 0;
      const auto vertex3Idx = i * kTileVerticesCount + 2;
      ASSERT(vertex1Idx < vertices.size());
      ASSERT(vertex3Idx < vertices.size());

      const auto& vertex1 = vertices.at(vertex1Idx);
      const auto& vertex3 = vertices.at(vertex3Idx);

      if (posX >= (vertex1.positions[0] - TileVertex::kTileSpacing) && posX <= vertex3.positions[0]) {
        if (posY >= (vertex1.positions[1] - TileVertex::kTileSpacing) && posY <= vertex3.positions[1]) {
          return static_cast<int>(vertex1Idx);
        }
      }
    }
    return -1;
  }
};

#endif