#ifndef SCENE_TRAINING_H_
#define SCENE_TRAINING_H_

#include "BaseSquareModel.h"
#include "CharacterModel.h"
#include "GameCharacter.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Shader.h"
#include "StreamVertexBuffer.h"
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"

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
  GameCharacter* character_;
  Renderer& renderer_;
  BaseSquareModel bsModel_;
  CharacterModel cModel_;
  float deltaTime_{0.0F};

  // For now we only render 1 main character
  static constexpr unsigned int kTexturesCount = 1;
  enum TextureIdx {
    MAIN_CHARACTER = 0,
  };
  std::array<std::unique_ptr<Texture>, kTexturesCount> textures_;

  std::unique_ptr<Shader> shader_{new Shader("res/shaders/scene_training.shader")};
  std::unique_ptr<VertexArray> va_{new VertexArray};
  std::unique_ptr<StreamVertexBuffer> vbVertices_{new StreamVertexBuffer(
      bsModel_.vertices_.data(), sizeof(bsModel_.vertices_), {GL_FLOAT, 2, GL_FALSE}, VBSUsage::ONCE, GL_STATIC_DRAW)};
  std::unique_ptr<StreamVertexBuffer> vbTextures_{new StreamVertexBuffer(
      bsModel_.textures_.data(), sizeof(bsModel_.textures_), {GL_FLOAT, 2, GL_FALSE}, VBSUsage::ONCE, GL_STATIC_DRAW)};

  // For now we only render 1 main character
  static constexpr unsigned int kMaxInstancesCount = 1;
  std::unique_ptr<StreamVertexBuffer> vbiPositions_{
      new StreamVertexBuffer(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount, {GL_FLOAT, 2, GL_FALSE})};
  std::unique_ptr<StreamVertexBuffer> vbiScales_{
      new StreamVertexBuffer(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount, {GL_FLOAT, 2, GL_FALSE})};
  std::unique_ptr<StreamVertexBuffer> vbiAngles_{
      new StreamVertexBuffer(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount, {GL_FLOAT, 1, GL_FALSE})};
  std::unique_ptr<StreamVertexBuffer> vbiTextureIds_{
      new StreamVertexBuffer(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount, {GL_FLOAT, 1, GL_FALSE})};

  auto currentInstanceCount() -> GLsizei {
    // For now we only render 1 main character
    return 1;
  }

  void setVBInstances();

  void onMoveCharacter();
};

}  // namespace test

#endif