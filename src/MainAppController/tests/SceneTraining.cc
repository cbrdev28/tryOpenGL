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

  renderer_.enableBlend();
  // Populate our textures array for each kind of entity to render (for now only 1 main character)
  textures_.at(TextureIdx::MAIN_CHARACTER) = std::make_unique<Texture>(character_->texturePath, true);

  // Match the order of shader layout!
  va_->setStreamBufferLayout({vbVertices_.get(), vbTextures_.get(), vbiPositions_.get(), vbiScales_.get(),
                              vbiAngles_.get(), vbiTextureIds_.get()});

  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto& aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  for (unsigned int i = 0; i < textures_.size(); ++i) {
    textures_.at(i)->bind(i);
    shader_->setUniform1i(fmt::format("u_textureSampler_{}", i), static_cast<GLint>(i));
  }

  shader_->unBind();
  va_->unBind();
  vbiTextureIds_->unBind();
  vbiAngles_->unBind();
  vbiScales_->unBind();
  vbiPositions_->unBind();
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
  renderer_.drawInstance(*shader_, *va_, static_cast<GLsizei>(bsModel_.vertices_.size()), this->currentInstanceCount());
}

void SceneTraining::onImGuiRender() {}

void SceneTraining::setVBInstances() {
  // GLintptr offset = 0;
  auto sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * 1);  // Only 1 main character
  vbiPositions_->setInstanceData(&cModel_.position, sizeToSend, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount);
  vbiPositions_->unBind();

  // offset = 0;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * 1);  // Only 1 main character
  vbiScales_->setInstanceData(&cModel_.scale, sizeToSend, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount);
  vbiScales_->unBind();

  // offset = 0;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * 1);  // Only 1 main character
  vbiAngles_->setInstanceData(&cModel_.angle, sizeToSend, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount);
  vbiAngles_->unBind();

  // offset = 0;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * 1);  // Only 1 main character
  vbiTextureIds_->setInstanceData(&cModel_.textureID, sizeToSend, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount);
  vbiTextureIds_->unBind();
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
  cModel_.position += glm::normalize(direction) * deltaTime_ * CharacterModel::kMoveSpeed;
  this->setVBInstances();
}

}  // namespace test