#ifndef BASE_SQUARE_MODEL_H_
#define BASE_SQUARE_MODEL_H_

struct BaseSquareModel {
  static constexpr GLfloat kBaseSize = 0.05F;
  static constexpr GLfloat kHalfBaseSize = 0.05F / 2.0F;
  static constexpr unsigned int kVertexCount = 6;

  std::array<glm::vec2, kVertexCount> vertices_ = {
      glm::vec2(-kHalfBaseSize, -kHalfBaseSize), glm::vec2(kHalfBaseSize, -kHalfBaseSize),
      glm::vec2(kHalfBaseSize, kHalfBaseSize),   glm::vec2(kHalfBaseSize, kHalfBaseSize),
      glm::vec2(-kHalfBaseSize, kHalfBaseSize),  glm::vec2(-kHalfBaseSize, -kHalfBaseSize),
  };
};

#endif  // BASE_SQUARE_MODEL_H_
