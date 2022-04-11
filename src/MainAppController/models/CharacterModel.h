#ifndef CHARACTER_MODEL_H_
#define CHARACTER_MODEL_H_

struct CharacterModel {
  glm::vec2 position = {0.0F, 0.0F};
  glm::vec2 scale = {1.0F, 1.0F};
  // To rotate around Z axis (most likely for weapons/bullets)
  float angle = glm::radians(0.0F);

  // To flip textures according to direction of character
  // glm::vec2 direction = {0.0F, 0.0F};

  // TODO(cbr): reserve value 0 for color? See when adding other squares (grunts, ...)
  // unsigned int textureID;
};

#endif  // CHARACTER_MODEL_H_
