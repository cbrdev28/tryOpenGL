#ifndef TILE_VERTEX_H
#define TILE_VERTEX_H

#include <array>
#include <vector>

struct TileVertex {
  static const unsigned int posCount = 2;
  static const unsigned int textureCoordCount = 2;

  std::array<float, posCount> positions;
  std::array<float, textureCoordCount> textureCoords;
  float textureIdx;

  inline static auto serialize(const std::vector<TileVertex>& tileVertices) -> std::vector<float> {
    // The goal is to transform a vector of TileVertex into a vector of float
    // Count how many floats we have in 1 TileVertex (used to reserve vector size)
    const auto numOfFloatValues = posCount + textureCoordCount + 1;

    std::vector<float> serializedTiles = {};
    serializedTiles.reserve(numOfFloatValues * tileVertices.size());
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