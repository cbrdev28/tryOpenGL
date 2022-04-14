#ifndef CHARACTER_MODEL_H_
#define CHARACTER_MODEL_H_

struct CharacterModel {
  static constexpr float kMoveSpeed{1.0F};

  static constexpr glm::vec2 position = {0.0F, 0.0F};
  static constexpr glm::vec2 scale = {3.0F, 3.0F};
  static constexpr GLfloat angle = glm::radians(0.0F);  // To rotate around Z axis (most likely for weapons/bullets)
  static constexpr GLfloat textureID = 0.0F;

  // To flip textures according to direction of character
  // glm::vec2 direction = {0.0F, 0.0F};
};

#endif  // CHARACTER_MODEL_H_
