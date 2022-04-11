#ifndef CHARACTER_MODEL_H_
#define CHARACTER_MODEL_H_

struct CharacterModel {
  glm::vec2 position = {0.0F, 0.0F};
  glm::vec2 scale = {1.0F, 1.0F};
  GLfloat angle = glm::radians(0.0F);  // To rotate around Z axis (most likely for weapons/bullets)
  GLfloat textureID = 0.0F;

  // To flip textures according to direction of character
  // glm::vec2 direction = {0.0F, 0.0F};
};

#endif  // CHARACTER_MODEL_H_
