#include "cbrpch.h"

#include "SceneTraining.h"

#include "VertexBufferLayout.h"

namespace test {

SceneTraining::SceneTraining(const TestContext& ctx)
    : Test(ctx), gameManager_(*ctx.gameManager), currentCharacter_(gameManager_.getCurrentCharacter()) {
  // This scene cannot work without a current character
  ASSERT(currentCharacter_ != nullptr);
  renderer_.enableBlend();
  textures_.at(TextureIdx::MAIN_CHARACTER) = std::make_unique<Texture>(currentCharacter_->texturePath);

  vbBaseVertices_ =
      std::make_unique<VertexBuffer>(baseSquareModel_.vertices_.data(), sizeof(baseSquareModel_.vertices_));
  vbBaseTextures_ =
      std::make_unique<VertexBuffer>(baseSquareModel_.textures_.data(), sizeof(baseSquareModel_.textures_));

  vbiPositions_ =
      std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW);
  vbiScales_ =
      std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW);
  vbiAngles_ =
      std::make_unique<VertexBuffer>(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW);
  vbiTextureIds_ =
      std::make_unique<VertexBuffer>(nullptr, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount, GL_STREAM_DRAW);

  vbBaseVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbBaseTextures_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbiPositions_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbiScales_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbiAngles_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbiTextureIds_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout verticesLayout;
  verticesLayout.pushFloat(2);  // Each vertex is made of 2 floats
  VertexBufferLayout texturesLayout;
  texturesLayout.pushFloat(2);  // Each texture coordinate is made of 2 floats
  VertexBufferLayout positionsLayout;
  positionsLayout.pushFloat(2);  // Each position is made of 2 floats
  VertexBufferLayout scalesLayout;
  scalesLayout.pushFloat(2);  // Each scale is made of 2 floats
  VertexBufferLayout anglesLayout;
  anglesLayout.pushFloat(1);  // Each angle is made of 1 float
  VertexBufferLayout textureIdsLayout;
  textureIdsLayout.pushFloat(1);  // Each texture ID is made of 1 float

  va_->setInstanceBufferLayout({
      // The order needs to match with our shader!
      {*vbBaseVertices_, verticesLayout},
      {*vbBaseTextures_, texturesLayout},
      {*vbiPositions_, positionsLayout},
      {*vbiScales_, scalesLayout},
      {*vbiAngles_, anglesLayout},
      {*vbiTextureIds_, textureIdsLayout},
  });

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
  vbBaseTextures_->unBind();
  vbBaseVertices_->unBind();

  this->setVBInstances();
}

SceneTraining::~SceneTraining() = default;

void SceneTraining::onUpdate(float /*deltaTime */) {}

void SceneTraining::onRender() {
  renderer_.drawInstance(*shader_, *va_, static_cast<GLsizei>(baseSquareModel_.vertices_.size()),
                         this->currentInstanceCount());
}

void SceneTraining::onImGuiRender() {}

void SceneTraining::setVBInstances() {
  GLintptr offset = 0;
  auto sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * 1);  // Only 1 main character
  vbiPositions_->setInstanceData(&characterModel_.position, sizeToSend,
                                 sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount);
  vbiPositions_->unBind();

  offset = 0;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * 1);  // Only 1 main character
  vbiScales_->setInstanceData(&characterModel_.scale, sizeToSend,
                              sizeof(glm::vec2) * SceneTraining::kMaxInstancesCount);
  vbiScales_->unBind();

  offset = 0;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * 1);  // Only 1 main character
  vbiAngles_->setInstanceData(&characterModel_.angle, sizeToSend, sizeof(GLfloat) * SceneTraining::kMaxInstancesCount);
  vbiAngles_->unBind();

  offset = 0;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * 1);  // Only 1 main character
  vbiTextureIds_->setInstanceData(&characterModel_.textureID, sizeToSend,
                                  sizeof(GLfloat) * SceneTraining::kMaxInstancesCount);
  vbiTextureIds_->unBind();
}

}  // namespace test