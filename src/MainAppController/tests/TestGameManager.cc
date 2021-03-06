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
  } else {
    this->getTestContext().renderer->clearColorBackground(0.2F, 0.5F, 0.3F, 1.0F);
  }
}

void TestGameManager::onImGuiRender() {
  if (currentScene_) {
    // if (ImGui::Button(fmt::format("Back to Game Manager [{}]", fmt::ptr(currentScene_)).c_str())) {
    if (ImGui::Button("Back to Game Menu")) {
      currentScene_.reset(nullptr);
      return;
    }
    ImGui::Separator();
    currentScene_->onImGuiRender();
    return;
  }

  auto gm = this->getTestContext().gameManager;
  auto* gc = gm->getCurrentCharacter();

  ImGui::Text("Scenes");
  ImGui::Indent();
  for (const auto& scene : scenes_) {
    // Disable button for scenes which require a valid current character
    ImGui::BeginDisabled(scene.dependsOnCurrentCharacter && gc == nullptr);
    if (ImGui::Button(scene.name.c_str())) {
      currentScene_ = scene.create();
    }
    ImGui::EndDisabled();
  }
  ImGui::Unindent();

  ImGui::Text("Character");
  ImGui::Indent();
  if (gc != nullptr) {
    ImGui::Text("%s", fmt::format("Name: {}", gc->name).c_str());
  } else {
    ImGui::Text("NONE");
  }
  ImGui::Unindent();

  ImGui::NewLine();
  ImGui::Text("Game stats");
  ImGui::Indent();
  ImGui::Text("Game clock: %.2f", gm->getGameTime());
  ImGui::Text("Time played: %.2f", gm->getTimePlayed());
  if (gm->isGameRunning()) {
    if (ImGui::Button("Stop clock")) {
      gm->stopGame();
    }
  } else {
    if (ImGui::Button("Start clock")) {
      gm->startGame();
    }
  }
  ImGui::Unindent();

  ImGui::Text("Window stats");
  ImGui::Indent();
  auto ws = this->getTestContext().windowManager->getWindowStats();
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  ImGui::Text("Avg FPS: %.2f",
              static_cast<float>(ws.frameCount) /
                  std::chrono::duration_cast<std::chrono::duration<float>>(ws.endTime - ws.startTime).count());
  ImGui::Unindent();
}

}  // namespace test