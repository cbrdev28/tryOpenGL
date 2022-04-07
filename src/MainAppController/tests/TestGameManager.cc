#include "cbrpch.h"

#include "TestGameManager.h"

namespace test {

TestGameManager::~TestGameManager() = default;

void TestGameManager::onUpdate(float deltaTime) { deltaTime_ = deltaTime; }

void TestGameManager::onRender() {}

void TestGameManager::onImGuiRender() {
  ImGui::NewLine();
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  // TODO(cbr): refactor and have a ref to WinStats
  auto frameCount = this->getTestContext().windowManager->getWindowStats().frameCount;
  auto startTime = this->getTestContext().windowManager->getWindowStats().startTime;
  auto endTime = this->getTestContext().windowManager->getWindowStats().endTime;
  ImGui::Text("Avg FPS: %.2f",
              static_cast<float>(frameCount) /
                  std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count());
}

}  // namespace test