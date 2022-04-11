#include "cbrpch.h"

#include "SceneTraining.h"

namespace test {

SceneTraining::SceneTraining(const TestContext& ctx)
    : Test(ctx), gameManager_(*ctx.gameManager), currentCharacter_(gameManager_.getCurrentCharacter()) {
  // This scene cannot work without a current character
  ASSERT(currentCharacter_ != nullptr);
  renderer_.enableBlend();
  textures_.at(TextureIdx::MAIN_CHARACTER) = std::make_unique<Texture>(currentCharacter_->texturePath);

  shader_->bind();
  for (unsigned int i = 0; i < textures_.size(); ++i) {
    textures_.at(i)->bind(i);
    shader_->setUniform1i(fmt::format("u_textureSampler_{}", i), static_cast<GLint>(i));
  }

  shader_->unBind();
}

SceneTraining::~SceneTraining() = default;

void SceneTraining::onUpdate(float /*deltaTime */) {}

void SceneTraining::onRender() {}

void SceneTraining::onImGuiRender() {}

}  // namespace test