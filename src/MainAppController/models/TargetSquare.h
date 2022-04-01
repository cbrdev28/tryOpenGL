#ifndef TARGET_SQUARE_H_
#define TARGET_SQUARE_H_

#include <array>

#include "../common/glmHeaders.h"
#include "../common/openGLHeaders.h"
#include "../helpers/openGLErrorHelpers.h"

struct TargetSquare {
  static constexpr GLfloat kSize = 0.1F;

  std::array<glm::vec2, 6> vertices = resetVertices();
  glm::vec2 position = {0.0F, 0.0F};
  GLfloat zAngle = glm::radians(45.0F);

  auto resetVertices(const GLfloat size = kSize) -> std::array<glm::vec2, 6>& {
    const auto halfSize = size / 2.0F;
    vertices.at(0) = glm::vec2(-halfSize, -halfSize);
    vertices.at(1) = glm::vec2(halfSize, -halfSize);
    vertices.at(2) = glm::vec2(halfSize, halfSize);

    vertices.at(3) = glm::vec2(halfSize, halfSize);
    vertices.at(4) = glm::vec2(-halfSize, halfSize);
    vertices.at(5) = glm::vec2(-halfSize, -halfSize);
    return vertices;
  }

  [[nodiscard]] auto verticesGLSize() const -> GLsizeiptr {
    return static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec2));
  }
};

#endif