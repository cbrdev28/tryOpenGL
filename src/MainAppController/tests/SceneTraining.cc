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

  vbVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbTextures_->setDivisor(VertexBufferDivisor::ALWAYS);
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
      {*vbVertices_, verticesLayout},
      {*vbTextures_, texturesLayout},
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
  vbTextures_->unBind();
  vbVertices_->unBind();

  this->setVBInstances();
}

SceneTraining::~SceneTraining() = default;

void SceneTraining::onUpdate(float /*deltaTime */) {}

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

}  // namespace test