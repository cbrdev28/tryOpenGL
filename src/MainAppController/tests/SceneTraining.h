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

struct SceneTrainingModel {
  unsigned int maxCount{1};
  // Static render data
  std::string texturePath;
  // Stream render data
  std::vector<glm::vec2> positions;
  std::vector<glm::vec2> scales;
  std::vector<GLfloat> angles;  // To rotate around Z axis
  std::vector<GLfloat> textureIDs;
  // Other data
  float moveSpeed{1.0F};

  SceneTrainingModel() {
    positions.reserve(maxCount);
    scales.reserve(maxCount);
    angles.reserve(maxCount);
    textureIDs.reserve(maxCount);
  }

  SceneTrainingModel(unsigned int count, std::string path) : maxCount(count), texturePath(std::move(path)) {
    positions.reserve(maxCount);
    scales.reserve(maxCount);
    angles.reserve(maxCount);
    textureIDs.reserve(maxCount);
  }

  [[nodiscard]] auto instancesCount() const -> GLintptr { return static_cast<GLintptr>(positions.size()); }
};

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
  float deltaTime_{0.0F};
  GameManager& gameManager_;
  GameCharacter* character_;
  Renderer& renderer_;

  // Keep track of model order to match with shader textures
  enum ModelIdx {
    MAIN_CHARACTER = 0,
  };
  static constexpr unsigned int kModelsCount = 1;
  std::array<SceneTrainingModel, kModelsCount> models_;
  std::array<std::unique_ptr<Texture>, kModelsCount> textures_;

  std::unique_ptr<Shader> shader_{new Shader("res/shaders/scene_training.shader")};
  std::unique_ptr<VertexArray> va_{new VertexArray};
  std::unique_ptr<StreamVertexBuffer> vbVertices_{
      new StreamVertexBuffer(BaseSquareModel::vertices_.data(), sizeof(BaseSquareModel::vertices_),
                             {GL_FLOAT, 2, GL_FALSE}, VBSUsage::ONCE, GL_STATIC_DRAW)};
  std::unique_ptr<StreamVertexBuffer> vbTextures_{
      new StreamVertexBuffer(BaseSquareModel::textures_.data(), sizeof(BaseSquareModel::textures_),
                             {GL_FLOAT, 2, GL_FALSE}, VBSUsage::ONCE, GL_STATIC_DRAW)};

  // For now we only render 1 main character
  static constexpr unsigned int kMaxInstancesCount = 1;
  // Keep track of model order to match with shader layouts
  enum SVBIdx {
    POSITIONS = 0,
    SCALES = 1,
    ANGLES = 2,
    TEXTURE_IDS = 3,
  };
  static constexpr unsigned int kVBSCount = 4;
  std::array<std::unique_ptr<StreamVertexBuffer>, kVBSCount> streamVbs_{
      std::make_unique<StreamVertexBuffer>(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount,
                                           VertexBufferElement{GL_FLOAT, 2, GL_FALSE}),
      std::make_unique<StreamVertexBuffer>(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount,
                                           VertexBufferElement{GL_FLOAT, 2, GL_FALSE}),
      std::make_unique<StreamVertexBuffer>(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount,
                                           VertexBufferElement{GL_FLOAT, 1, GL_FALSE}),
      std::make_unique<StreamVertexBuffer>(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount,
                                           VertexBufferElement{GL_FLOAT, 1, GL_FALSE}),

  };

  auto currentInstanceCount() -> GLsizei {
    // For now we only render 1 main character
    return 1;
  }

  void setVBInstances();
  void onMoveCharacter();

  template <typename Src>
  auto setModelStreamData(const std::vector<Src>& src, const StreamVertexBuffer& svb, GLintptr elementOffset) -> void {
    constexpr auto modelSizeof = sizeof(Src);
    auto byteOffset = elementOffset * modelSizeof;
    auto sizeToSend = static_cast<GLsizeiptr>(modelSizeof * src.size());
    svb.setInstanceDataOffset(src.data(), sizeToSend, 0, byteOffset);
  }
};

}  // namespace test

#endif