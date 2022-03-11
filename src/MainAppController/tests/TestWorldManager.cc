#include "TestWorldManager.h"

#include <imgui.h>

namespace test {
TestWorldManager::TestWorldManager(const TestContext& ctx) : Test(ctx) {
  worldManager_.init(static_cast<float>(ctx.windowManager.getWidth()),
                     static_cast<float>(ctx.windowManager.getHeight()));
  ctx.windowManager.addWindowListener(&worldManager_);
  ctx.inputManager.addKeyboardListener(&worldManager_);
}

TestWorldManager::~TestWorldManager() {
  this->getTestContext().windowManager.removeWindowListener(&worldManager_);
  this->getTestContext().inputManager.removeKeyboardListener(&worldManager_);
}

void TestWorldManager::onUpdate(float deltaTime) {}
void TestWorldManager::onRender() { worldManager_.render(); }
void TestWorldManager::onImGuiRender() { ImGui::ColorEdit4("Color", worldManager_.getBackGroundColor()->data()); }
}  // namespace test
