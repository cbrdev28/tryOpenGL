#include "cbrpch.h"

#include "SceneSelectCharacter.h"

namespace test {

SceneSelectCharacter::SceneSelectCharacter(const TestContext& ctx) : Test(ctx) {
  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto& aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));
  shader_->unBind();
}

SceneSelectCharacter::~SceneSelectCharacter() = default;

void SceneSelectCharacter::onUpdate(float /*deltaTime */) {}

void SceneSelectCharacter::onRender() { renderer_.clearColorBackground(backgroundColor_); }

void SceneSelectCharacter::onImGuiRender() {
  auto gm = this->getTestContext().gameManager;

  ImGui::Text("Your character");
  ImGui::Indent();
  auto* gc = gm->getCurrentCharacter();
  if (gc != nullptr) {
    ImGui::Text("%s", fmt::format("Name: {}", gc->name).c_str());
  } else {
    ImGui::Text("NONE");
  }
  ImGui::Unindent();

  ImGui::Text("Choose character");
  ImGui::Indent();
  for (const auto& character : characters_) {
    if (ImGui::Button(character.name.c_str())) {
      gm->setCurrentCharacter(character);
    }
  }
  ImGui::Unindent();

  ImGui::Text("Background color");
  ImGui::Indent();
  ImGui::ColorEdit4("", backgroundColor_.data());
  ImGui::Unindent();
}

}  // namespace test