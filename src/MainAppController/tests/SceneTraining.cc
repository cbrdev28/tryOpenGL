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
      std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * SceneTraining::kInstancesCount, GL_STREAM_DRAW);
  vbiScales_ =
      std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * SceneTraining::kInstancesCount, GL_STREAM_DRAW);

  vbBaseVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbBaseTextures_->setDivisor(VertexBufferDivisor::ALWAYS);

  VertexBufferLayout verticesLayout;
  verticesLayout.pushFloat(2);  // Each vertex is made of 2 floats
  VertexBufferLayout texturesLayout;
  texturesLayout.pushFloat(2);  // Each texture coordinate is made of 2 floats
  VertexBufferLayout positionsLayout;
  positionsLayout.pushFloat(2);  // Each position is made of 2 floats
  VertexBufferLayout scalesLayout;
  scalesLayout.pushFloat(2);  // Each scale is made of 2 floats

  va_->setInstanceBufferLayout({
      // The order needs to match with our shader!
      {*vbBaseVertices_, verticesLayout},
      {*vbBaseTextures_, texturesLayout},
      {*vbiPositions_, positionsLayout},
      {*vbiScales_, scalesLayout},
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
  vbiScales_->unBind();
  vbiPositions_->unBind();
  vbBaseTextures_->unBind();
  vbBaseVertices_->unBind();
}

SceneTraining::~SceneTraining() = default;

void SceneTraining::onUpdate(float /*deltaTime */) {}

void SceneTraining::onRender() {}

void SceneTraining::onImGuiRender() {}

}  // namespace test