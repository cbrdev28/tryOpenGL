#include "cbrpch.h"

#include "TestGameManager.h"

namespace test {

TestGameManager::~TestGameManager() = default;

void TestGameManager::onUpdate(float deltaTime) { deltaTime_ = deltaTime; }

void TestGameManager::onRender() {}

void TestGameManager::onImGuiRender() {
  ImGui::NewLine();
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);

  auto ws = this->getTestContext().windowManager->getWindowStats();
  ImGui::Text("Avg FPS: %.2f",
              static_cast<float>(ws.frameCount) /
                  std::chrono::duration_cast<std::chrono::duration<float>>(ws.endTime - ws.startTime).count());

  ImGui::NewLine();
  auto gm = this->getTestContext().gameManager;
  ImGui::Text("Game clock: %.2f", gm->getGameTime());
  ImGui::Text("Time played: %.2f", gm->getTimePlayed());
  if (gm->isGameRunning()) {
    if (ImGui::Button("Stop game")) {
      gm->stopGame();
    }
  } else {
    if (ImGui::Button("Start game")) {
      gm->startGame();
    }
  }
}

}  // namespace test