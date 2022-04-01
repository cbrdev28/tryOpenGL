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
  static constexpr GLsizei kMaxTriangles = 1000;
  static constexpr GLfloat kSize = 0.1F;
  static constexpr GLfloat kRotationSpeed = 1.0F;

  std::array<glm::vec2, 3> vertices = resetVertices();
  std::vector<glm::vec2> positions;
  std::vector<GLfloat> zRotationAngles;

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

  void addTriangle() {
    const auto currentNumberOfTriangles = positions.size();
    ASSERT(currentNumberOfTriangles < kMaxTriangles);
    // Random positions between: -0.9 & 0.9
    const auto expectedValueRange = 0.9F;
    const auto randomPosX = expectedValueRange - (2 * genRandom() * expectedValueRange);
    const auto randomPosY = expectedValueRange - (2 * genRandom() * expectedValueRange);

    positions.emplace_back(randomPosX, randomPosY);
    zRotationAngles.emplace_back(0.0F);
  }

  void spawnTriangle(const glm::vec2& targetPosition) {
    const auto currentNumberOfTriangles = positions.size();
    ASSERT(currentNumberOfTriangles < kMaxTriangles);
    // Random positions between: -0.9 & 0.9
    const auto expectedValueRange = 0.9F;
    auto randomPosX = expectedValueRange - (2 * genRandom() * expectedValueRange);
    auto randomPosY = expectedValueRange - (2 * genRandom() * expectedValueRange);

    // If the triangle is within "a close" range from the target, push it away
    const float rangeOffset = 0.5F;
    glm::vec2 deltaTriangleTarget = {randomPosX - targetPosition.x, randomPosY - targetPosition.y};
    if (glm::abs(deltaTriangleTarget.x) < rangeOffset && glm::abs(deltaTriangleTarget.y) < rangeOffset) {
      float xMultiplier = deltaTriangleTarget.x > 0 ? 1.0F : -1.0F;
      float yMultiplier = deltaTriangleTarget.y > 0 ? 1.0F : -1.0F;
      float xOffset = rangeOffset - glm::abs(deltaTriangleTarget.x);
      float yOffset = rangeOffset - glm::abs(deltaTriangleTarget.y);

      randomPosX += xOffset * xMultiplier;
      randomPosY += yOffset * yMultiplier;
    }

    positions.emplace_back(randomPosX, randomPosY);
    zRotationAngles.emplace_back(0.0F);
  }

  void removeTriangle() {
    positions.pop_back();
    zRotationAngles.pop_back();
  }

  void onUpdateRotationAngle(float dt) {
    for (auto& angle : zRotationAngles) {
      angle += 1.0F * kRotationSpeed * dt;
    }
  }
};

#endif