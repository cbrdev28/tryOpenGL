#ifndef SCENE_TRAINING_H_
#define SCENE_TRAINING_H_

#include "BaseSquareModel.h"
#include "CharacterModel.h"
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
  GameCharacter* character_;
  BaseSquareModel bsModel_;
  CharacterModel cModel_;

  enum TextureIdx {
    MAIN_CHARACTER = 0,
  };
  // For now we only render 1 main character
  static constexpr unsigned int kTexturesCount = 1;
  std::array<std::unique_ptr<Texture>, kTexturesCount> textures_;

  Renderer renderer_;
  std::unique_ptr<Shader> shader_{new Shader("res/shaders/scene_training.shader")};
  std::unique_ptr<VertexArray> va_{new VertexArray};
  std::unique_ptr<VertexBuffer> vbVertices_{new VertexBuffer(bsModel_.vertices_.data(), sizeof(bsModel_.vertices_))};
  std::unique_ptr<VertexBuffer> vbTextures_{new VertexBuffer(bsModel_.textures_.data(), sizeof(bsModel_.textures_))};

  // For now we only render 1 main character
  static constexpr unsigned int kMaxInstancesCount = 1;
  std::unique_ptr<VertexBuffer> vbiPositions_{
      new VertexBuffer(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW)};
  std::unique_ptr<VertexBuffer> vbiScales_{
      new VertexBuffer(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW)};
  std::unique_ptr<VertexBuffer> vbiAngles_{
      new VertexBuffer(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW)};
  std::unique_ptr<VertexBuffer> vbiTextureIds_{
      new VertexBuffer(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW)};

  auto currentInstanceCount() -> GLsizei {
    // For now we only render 1 main character
    return 1;
  }
  void setVBInstances();
};

}  // namespace test

#endif