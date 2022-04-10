#include "cbrpch.h"

#include "SceneTraining.h"

#include "openGLErrorHelpers.h"

namespace test {

SceneTraining::SceneTraining(const TestContext& ctx)
    : Test(ctx), gameManager_(*ctx.gameManager), currentCharacter_(gameManager_.getCurrentCharacter()) {
  // This scene cannot work without a current character
  ASSERT(currentCharacter_ != nullptr);
}

SceneTraining::~SceneTraining() = default;

void SceneTraining::onUpdate(float /*deltaTime */) {}

void SceneTraining::onRender() {}

void SceneTraining::onImGuiRender() {}

}  // namespace test