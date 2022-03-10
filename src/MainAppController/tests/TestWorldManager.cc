#include "TestWorldManager.h"

#include <imgui.h>

namespace test {
TestWorldManager::TestWorldManager() { worldManager_.init(); }

void TestWorldManager::onUpdate(float deltaTime) {}
void TestWorldManager::onRender() { worldManager_.render(); }
void TestWorldManager::onImGuiRender() { ImGui::ColorEdit4("Color", worldManager_.getBackGroundColor()->data()); }
}  // namespace test
