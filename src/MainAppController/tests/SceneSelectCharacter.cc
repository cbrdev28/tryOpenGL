#include "cbrpch.h"

#include "SceneSelectCharacter.h"

namespace test {

SceneSelectCharacter::~SceneSelectCharacter() = default;

void SceneSelectCharacter::onUpdate(float deltaTime) {}

void SceneSelectCharacter::onRender() { renderer_.clearColorBackground(backgroundColor_); }

void SceneSelectCharacter::onImGuiRender() {
  ImGui::Text("Your character");
  ImGui::Indent();
  auto* gc = this->getTestContext().gameManager->getCurrentCharacter();
  if (gc != nullptr) {
    ImGui::Text("%s", fmt::format("Name: {}", gc->name).c_str());
  } else {
    ImGui::Text("NONE");
  }
  ImGui::Unindent();

  ImGui::Text("Background color");
  ImGui::Indent();
  ImGui::ColorEdit4("", backgroundColor_.data());
}

}  // namespace test