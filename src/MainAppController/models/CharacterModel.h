#ifndef CHARACTER_MODEL_H_
#define CHARACTER_MODEL_H_

struct CharacterModel {
  glm::vec2 position = {0.0F, 0.0F};
  glm::vec2 scale = {1.0F, 1.0F};

  // TODO(cbr): reserve value 0 for color? See when adding other squares (grunts, ...)
  // unsigned int textureID;
};

#endif  // CHARACTER_MODEL_H_
