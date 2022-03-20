#ifndef INSTANCED_TRIANGLE_H_
#define INSTANCED_TRIANGLE_H_

#include <array>

#include "../common/glmHeaders.h"
#include "../common/openGLHeaders.h"

struct InstancedTriangle {
  static constexpr GLfloat kSize = 0.5F;

  std::array<glm::vec2, 3> vertices{};
  std::array<GLuint, 3> indices = {0, 1, 2};

  void makeVertices(const GLfloat size = kSize) {
    const auto halfSize = size / 2.0F;
    vertices.at(0) = glm::vec2(-halfSize, -halfSize);
    vertices.at(1) = glm::vec2(halfSize, -halfSize);
    vertices.at(2) = glm::vec2(0.0F, halfSize);
  }

  auto verticesGLSize() -> GLsizeiptr { return static_cast<GLsizeiptr>(vertices.size() * 2 * sizeof(GLfloat)); }
};

#endif