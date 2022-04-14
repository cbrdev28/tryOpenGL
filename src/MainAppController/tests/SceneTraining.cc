#include "cbrpch.h"

#include "SceneTraining.h"

#include "VertexBufferLayout.h"

namespace test {

SceneTraining::SceneTraining(const TestContext& ctx)
    : Test(ctx),
      gameManager_(*ctx.gameManager),
      character_(gameManager_.getCurrentCharacter()),
      renderer_(*ctx.renderer) {
  // This scene cannot work without a current character
  ASSERT(character_ != nullptr);

  SceneTrainingModel mainCharacterModel;
  mainCharacterModel.texturePath = character_->texturePath;
  mainCharacterModel.positions.emplace_back(CharacterModel::position);
  mainCharacterModel.scales.emplace_back(CharacterModel::scale);
  mainCharacterModel.angles.emplace_back(CharacterModel::angle);
  // Populate model in order to match with textures in shader
  mainCharacterModel.textureIDs.emplace_back(ModelIdx::MAIN_CHARACTER);
  models_.at(ModelIdx::MAIN_CHARACTER) = mainCharacterModel;

  // Match the order of shader layout!
  va_->setStreamBufferLayout({vbVertices_.get(), vbTextures_.get(), streamVbs_.at(SVBIdx::POSITIONS).get(),
                              streamVbs_.at(SVBIdx::SCALES).get(), streamVbs_.at(SVBIdx::ANGLES).get(),
                              streamVbs_.at(SVBIdx::TEXTURE_IDS).get()});

  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto& aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  renderer_.enableBlend();
  // Populate our textures array for each kind of entity to render
  for (unsigned int i = 0; i < models_.size(); ++i) {
    textures_.at(i) = std::make_unique<Texture>(models_.at(i).texturePath, true);
    textures_.at(i)->bind(i);
    shader_->setUniform1i(fmt::format("u_textureSampler_{}", i), static_cast<GLint>(i));
  }

  shader_->unBind();
  va_->unBind();
  for (const auto& svb : streamVbs_) {
    svb->unBind();
  }
  vbTextures_->unBind();
  vbVertices_->unBind();

  this->setVBInstances();
}

SceneTraining::~SceneTraining() = default;

void SceneTraining::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;
  this->onMoveCharacter();
}

void SceneTraining::onRender() {
  renderer_.drawInstance(*shader_, *va_, static_cast<GLsizei>(BaseSquareModel::vertices_.size()),
                         this->currentInstanceCount());
}

void SceneTraining::onImGuiRender() {}

void SceneTraining::setVBInstances() {
  GLintptr offset = 0;
  for (const auto& model : models_) {
    this->setModelStreamData(model.positions, *streamVbs_.at(0), offset);
    this->setModelStreamData(model.scales, *streamVbs_.at(1), offset);
    this->setModelStreamData(model.angles, *streamVbs_.at(2), offset);
    this->setModelStreamData(model.textureIDs, *streamVbs_.at(3), offset);
    offset += model.instancesCount();
  }
  for (const auto& svb : streamVbs_) {
    svb->unBind();
  }
}

void SceneTraining::onMoveCharacter() {
  const auto& wm = this->getTestContext().windowManager;
  const bool& keyWDown = wm->getPressedDownKeysMap().at(GLFW_KEY_W);
  const bool& keyADown = wm->getPressedDownKeysMap().at(GLFW_KEY_A);
  const bool& keySDown = wm->getPressedDownKeysMap().at(GLFW_KEY_S);
  const bool& keyDDown = wm->getPressedDownKeysMap().at(GLFW_KEY_D);
  if (!keyWDown && !keyADown && !keySDown && !keyDDown) {
    return;
  }

  glm::vec2 direction{0.0F, 0.0F};
  if (keyWDown) {
    direction += glm::vec2{0.0F, 1.0F};
  }
  if (keySDown) {
    direction += glm::vec2{0.0F, -1.0F};
  }
  if (keyADown) {
    direction += glm::vec2{-1.0F, 0.0F};
  }
  if (keyDDown) {
    direction += glm::vec2{1.0F, 0.0F};
  }

  if (direction == glm::vec2{0.0F, 0.0F}) {
    return;
  }

  models_.at(ModelIdx::MAIN_CHARACTER).positions.at(0) +=
      glm::normalize(direction) * deltaTime_ * CharacterModel::kMoveSpeed;
  this->setVBInstances();
}

}  // namespace test