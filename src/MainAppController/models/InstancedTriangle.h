#ifndef INSTANCED_TRIANGLE_H_
#define INSTANCED_TRIANGLE_H_

#include <array>
#include <cstddef>
#include <random>
#include <vector>

#include "../common/glmHeaders.h"
#include "../common/openGLHeaders.h"
#include "../helpers/openGLErrorHelpers.h"

struct InstancedTriangle {
  static constexpr GLsizei kMaxTriangles = 64;
  static constexpr GLfloat kSize = 0.1F;

  std::array<glm::vec2, 3> vertices = resetVertices();
  std::array<GLuint, 3> indices = {0, 1, 2};

  auto resetVertices(const GLfloat size = kSize) -> std::array<glm::vec2, 3>& {
    const auto halfSize = size / 2.0F;
    vertices.at(0) = glm::vec2(-halfSize, -halfSize);
    vertices.at(1) = glm::vec2(halfSize, -halfSize);
    vertices.at(2) = glm::vec2(0.0F, halfSize);
    return vertices;
  }

  auto verticesGLSize() -> GLsizeiptr { return static_cast<GLsizeiptr>(vertices.size() * 2 * sizeof(GLfloat)); }

  std::default_random_engine gen{std::random_device{}()};
  auto genRandom() -> GLfloat {
    std::uniform_real_distribution<GLfloat> dist(0.0F, 1.0F);
    return dist(gen);
  }

  std::vector<glm::vec2> positions{};

  auto addTriangle() -> std::vector<glm::vec2>& {
    // The number of triangles is the number of positions
    const auto currentNumberOfTriangles = positions.size();
    ASSERT(currentNumberOfTriangles <= kMaxTriangles);

    const auto expectedValueRange = 0.9F;
    const auto randomPosX = expectedValueRange - (2 * genRandom() * expectedValueRange);
    const auto randomPosY = expectedValueRange - (2 * genRandom() * expectedValueRange);

    positions.emplace_back(glm::vec2(randomPosX, randomPosY));
    return positions;
  }

  auto removeTriangle() -> std::vector<glm::vec2>& {
    positions.pop_back();
    return positions;
  }

  auto positionsGLSize() -> GLsizeiptr {
    return static_cast<GLsizeiptr>(positions.size()) * static_cast<GLsizeiptr>(sizeof(GLfloat) * 2);
  }

  auto maxPositionsGLSize() -> GLsizeiptr {
    return static_cast<GLsizeiptr>(kMaxTriangles) * static_cast<GLsizeiptr>(sizeof(GLfloat) * 2);
  }
};

#endif