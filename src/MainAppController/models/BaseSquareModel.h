#ifndef BASE_SQUARE_MODEL_H_
#define BASE_SQUARE_MODEL_H_

struct BaseSquareModel {
  static constexpr GLfloat kBaseSize = 0.08F;
  static constexpr GLfloat kHalfBaseSize = kBaseSize / 2.0F;
  static constexpr unsigned int kVertexCount = 6;

  std::array<glm::vec2, kVertexCount> vertices_ = {
      glm::vec2(-kHalfBaseSize, -kHalfBaseSize), glm::vec2(kHalfBaseSize, -kHalfBaseSize),
      glm::vec2(kHalfBaseSize, kHalfBaseSize),   glm::vec2(kHalfBaseSize, kHalfBaseSize),
      glm::vec2(-kHalfBaseSize, kHalfBaseSize),  glm::vec2(-kHalfBaseSize, -kHalfBaseSize),
  };

  std::array<glm::vec2, kVertexCount> textures_ = {glm::vec2(0.0F, 0.0F), glm::vec2(1.0F, 0.0F), glm::vec2(1.0F, 1.0F),
                                                   glm::vec2(1.0F, 1.0F), glm::vec2(0.0F, 1.0F), glm::vec2(0.0F, 0.0F)};
};

#endif  // BASE_SQUARE_MODEL_H_
