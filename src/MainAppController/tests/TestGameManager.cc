#include "cbrpch.h"

#include "TestGameManager.h"

namespace test {

TestGameManager::~TestGameManager() = default;

void TestGameManager::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;
  if (currentScene_) {
    currentScene_->onUpdate(deltaTime);
  }
}

void TestGameManager::onRender() {
  if (currentScene_) {
    currentScene_->onRender();
  }
}

void TestGameManager::onImGuiRender() {
  if (currentScene_) {
    if (ImGui::Button(fmt::format("Back to Game Manager [{}]", fmt::ptr(currentScene_)).c_str())) {
      currentScene_.reset(nullptr);
      return;
    }
    ImGui::Separator();
    currentScene_->onImGuiRender();
    return;
  }

  ImGui::Text("Window Stats");
  ImGui::Indent();
  auto ws = this->getTestContext().windowManager->getWindowStats();
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  ImGui::Text("Avg FPS: %.2f",
              static_cast<float>(ws.frameCount) /
                  std::chrono::duration_cast<std::chrono::duration<float>>(ws.endTime - ws.startTime).count());
  ImGui::Unindent();

  ImGui::Text("Game Stats");
  ImGui::Indent();
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
  ImGui::Unindent();

  ImGui::Text("Scenes");
  ImGui::Indent();
  for (const auto& scene : scenes_) {
    if (ImGui::Button(scene.name.c_str())) {
      currentScene_ = scene.create();
    }
  }
  ImGui::Unindent();
}

}  // namespace test