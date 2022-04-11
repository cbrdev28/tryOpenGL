#ifndef SCENE_TRAINING_H_
#define SCENE_TRAINING_H_

// #include "CharacterModel.h"
#include "BaseSquareModel.h"
#include "GameCharacter.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Shader.h"
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {

class SceneTraining : public Test {
 public:
  explicit SceneTraining(const TestContext& ctx);
  ~SceneTraining() override;
  SceneTraining(const SceneTraining& other) = delete;
  SceneTraining(SceneTraining&& other) = delete;
  auto operator=(const SceneTraining& other) -> SceneTraining& = delete;
  auto operator=(SceneTraining&& other) -> SceneTraining& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  GameManager& gameManager_;
  GameCharacter* currentCharacter_;

  Renderer renderer_;
  std::unique_ptr<Shader> shader_{new Shader("res/shaders/scene_training.shader")};
  std::unique_ptr<VertexArray> va_{new VertexArray};
  std::unique_ptr<VertexBuffer> vbBaseVertices_;
  std::unique_ptr<VertexBuffer> vbBaseTextures_;

  enum TextureIdx {
    MAIN_CHARACTER = 0,
  };
  static constexpr unsigned int kTexturesCount = 1;
  std::array<std::unique_ptr<Texture>, kTexturesCount> textures_;

  BaseSquareModel baseSquareModel_;
  // CharacterModel characterModel_;
};

}  // namespace test

#endif