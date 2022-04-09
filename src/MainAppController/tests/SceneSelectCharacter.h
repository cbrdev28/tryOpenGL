#ifndef SCENE_SELECT_CHARACTER_H_
#define SCENE_SELECT_CHARACTER_H_

#include "GameCharacter.h"
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
  Renderer renderer_;
  std::array<float, 4> backgroundColor_{0.0F, 0.0F, 0.0F, 1.0F};
  std::array<GameCharacter, 2> characters_{{{"Clara"}, {"Felix"}}};

  std::unique_ptr<VertexArray> va_ = std::make_unique<VertexArray>();
  std::unique_ptr<Shader> shader_ = std::make_unique<Shader>("res/shaders/scene_select_character.shader");
  std::unique_ptr<VertexBuffer> vbVertices_;
  std::unique_ptr<VertexBuffer> vbTextures_;
  std::unique_ptr<Texture> texture_ = std::make_unique<Texture>("res/textures/grass.png");

  static constexpr GLfloat kBaseSize = 0.5F;
  std::array<glm::vec2, 6> vertices_ = {
      glm::vec2(-kBaseSize, -kBaseSize), glm::vec2(kBaseSize, -kBaseSize), glm::vec2(kBaseSize, kBaseSize),
      glm::vec2(kBaseSize, kBaseSize),   glm::vec2(-kBaseSize, kBaseSize), glm::vec2(-kBaseSize, -kBaseSize),
  };

  std::array<glm::vec2, 6> textures_ = {glm::vec2(0.0F, 0.0F), glm::vec2(1.0F, 0.0F), glm::vec2(1.0F, 1.0F),
                                        glm::vec2(1.0F, 1.0F), glm::vec2(0.0F, 1.0F), glm::vec2(0.0F, 0.0F)};
};

}  // namespace test

#endif