#include "TestWorldManager.h"

#include <imgui.h>

namespace test {
TestWorldManager::TestWorldManager(const TestContext& ctx) : Test(ctx) {
  worldManager_.init(static_cast<float>(ctx.windowManager->getWidth()),
                     static_cast<float>(ctx.windowManager->getHeight()));
  ctx.windowManager->addWindowListener(&worldManager_);
  ctx.inputManager->addKeyboardListener(&worldManager_);
}

TestWorldManager::~TestWorldManager() {
  this->getTestContext().windowManager->removeWindowListener(&worldManager_);
  this->getTestContext().inputManager->removeKeyboardListener(&worldManager_);
}

void TestWorldManager::onUpdate(float deltaTime) { worldManager_.setDeltaTimeFrame(deltaTime); }

void TestWorldManager::onRender() { worldManager_.render(); }

void TestWorldManager::onImGuiRender() {
  ImGui::Text("World frame time: %.3f", worldManager_.getDeltaTimeFrame());

  auto stats = this->getTestContext().windowManager->getWindowStats();
  ImGui::Text("Stat FPS: %.3f", 1.0F / stats.frameDeltaTime.count());

  auto totalTime = std::chrono::duration_cast<std::chrono::duration<float>>(stats.endTime - stats.startTime).count();
  ImGui::Text("Total time: %.2f", totalTime);
  ImGui::Text("Avg FPS: %.2f", static_cast<float>(stats.frameCount) / totalTime);

  ImGui::ColorEdit4("Color", worldManager_.getBackGroundColor()->data());
}

}  // namespace test
