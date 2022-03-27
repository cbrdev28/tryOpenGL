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
  std::vector<glm::vec2> positions{};
  std::vector<GLfloat> zRotationAngles{};

  InstancedTriangle() {
    positions.reserve(kMaxTriangles / 2);
    zRotationAngles.reserve(kMaxTriangles / 2);
  }

  [[nodiscard]] auto verticesGLSize() const -> GLsizeiptr {
    return static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec2));
  }

  [[nodiscard]] auto positionsGLSize() const -> GLsizeiptr {
    return static_cast<GLsizeiptr>(positions.size() * sizeof(glm::vec2));
  }
  [[nodiscard]] auto maxPositionsGLSize() const -> GLsizeiptr {
    return static_cast<GLsizeiptr>(kMaxTriangles * sizeof(glm::vec2));
  }

  [[nodiscard]] auto zRotationAnglesGLSize() const -> GLsizeiptr {
    return static_cast<GLsizeiptr>(zRotationAngles.size() * sizeof(GLfloat));
  }
  [[nodiscard]] auto maxZRotationAnglesGLSize() const -> GLsizeiptr {
    return static_cast<GLsizeiptr>(kMaxTriangles * sizeof(GLfloat));
  }

  auto resetVertices(const GLfloat size = kSize) -> std::array<glm::vec2, 3>& {
    const auto halfSize = size / 2.0F;
    vertices.at(0) = glm::vec2(-halfSize, -halfSize);
    vertices.at(1) = glm::vec2(halfSize, -halfSize);
    vertices.at(2) = glm::vec2(0.0F, halfSize);
    return vertices;
  }

  std::default_random_engine gen{std::random_device{}()};
  auto genRandom() -> GLfloat {
    std::uniform_real_distribution<GLfloat> dist(0.0F, 1.0F);
    return dist(gen);
  }

  // Adding a new triangle is making new:
  // - positions
  // - transformation matrix
  auto addTriangle() -> std::vector<glm::vec2>& {
    const auto currentNumberOfTriangles = positions.size();
    ASSERT(currentNumberOfTriangles < kMaxTriangles);
    // Random positions between: -0.9 & 0.9
    const auto expectedValueRange = 0.9F;
    const auto randomPosX = expectedValueRange - (2 * genRandom() * expectedValueRange);
    const auto randomPosY = expectedValueRange - (2 * genRandom() * expectedValueRange);

    positions.emplace_back(randomPosX, randomPosY);
    zRotationAngles.emplace_back(45.0F);
    return positions;
  }

  auto removeTriangle() -> std::vector<glm::vec2>& {
    positions.pop_back();
    zRotationAngles.pop_back();
    return positions;
  }
};

#endif