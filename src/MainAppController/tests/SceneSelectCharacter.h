#ifndef SCENE_SELECT_CHARACTER_H_
#define SCENE_SELECT_CHARACTER_H_

#include "GameCharacter.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Shader.h"
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {

class SceneSelectCharacter : public Test {
 public:
  explicit SceneSelectCharacter(const TestContext& ctx);
  ~SceneSelectCharacter() override;
  SceneSelectCharacter(const SceneSelectCharacter& other) = delete;
  SceneSelectCharacter(SceneSelectCharacter&& other) = delete;
  auto operator=(const SceneSelectCharacter& other) -> SceneSelectCharacter& = delete;
  auto operator=(SceneSelectCharacter&& other) -> SceneSelectCharacter& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  static constexpr unsigned int kCharacterCount = 2;
  static constexpr GLfloat kBaseSize = 0.5F;
  static constexpr unsigned int kVertexCount = 6;

  GameManager& gameManager_;
  std::array<float, 4> backgroundColor_{0.1F, 0.4F, 0.2F, 1.0F};
  std::array<GameCharacter, kCharacterCount> characters_{
      {{"Clara", "res/textures/car.png"}, {"Felix", "res/textures/dino.png"}}};

  std::array<glm::vec2, kVertexCount> vertices_ = {
      glm::vec2(-kBaseSize, -kBaseSize), glm::vec2(kBaseSize, -kBaseSize), glm::vec2(kBaseSize, kBaseSize),
      glm::vec2(kBaseSize, kBaseSize),   glm::vec2(-kBaseSize, kBaseSize), glm::vec2(-kBaseSize, -kBaseSize),
  };
  std::array<glm::vec2, kVertexCount> textures_ = {glm::vec2(0.0F, 0.0F), glm::vec2(1.0F, 0.0F), glm::vec2(1.0F, 1.0F),
                                                   glm::vec2(1.0F, 1.0F), glm::vec2(0.0F, 1.0F), glm::vec2(0.0F, 0.0F)};

  Renderer renderer_;
  std::unique_ptr<VertexArray> va_{new VertexArray};
  std::unique_ptr<Shader> shader_{new Shader("res/shaders/scene_select_character.shader")};
  std::unique_ptr<VertexBuffer> vbVertices_{new VertexBuffer(vertices_.data(), sizeof(vertices_))};
  std::unique_ptr<VertexBuffer> vbTextures_{new VertexBuffer(textures_.data(), sizeof(textures_))};
  std::unique_ptr<Texture> defaultTexture_{new Texture("res/textures/grass.png", false)};
  std::unique_ptr<Texture> currentTexture_ = nullptr;
};

}  // namespace test

#endif