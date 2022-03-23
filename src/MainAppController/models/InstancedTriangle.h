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
  std::vector<glm::mat4> transformations{};

  InstancedTriangle() {
    positions.reserve(kMaxTriangles / 2);
    transformations.reserve(kMaxTriangles / 2);
  }

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
    transformations.emplace_back(glm::translate(glm::mat4(1.0F), glm::vec3(randomPosX, randomPosY, 0.0)));
    return positions;
  }

  auto removeTriangle() -> std::vector<glm::vec2>& {
    positions.pop_back();
    transformations.pop_back();
    return positions;
  }

  auto transformationsGLSize() -> GLsizeiptr {
    return static_cast<GLsizeiptr>(transformations.size()) * static_cast<GLsizeiptr>(sizeof(GLfloat) * 16);
  }

  auto maxTransformationsGLSize() -> GLsizeiptr {
    return static_cast<GLsizeiptr>(kMaxTriangles) * static_cast<GLsizeiptr>(sizeof(GLfloat) * 16);
  }

  // Make triangle rotate
  void updateTransformation(float deltaTime) {
    for (auto& transformation : transformations) {
      transformation = glm::rotate(transformation, glm::radians(deltaTime * 45.0F), glm::vec3(0.0F, 0.0F, 1.0F));
    }
  }
};

#endif