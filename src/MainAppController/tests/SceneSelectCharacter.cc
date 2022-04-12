#include "cbrpch.h"

#include "SceneSelectCharacter.h"

#include "VertexBufferLayout.h"

namespace test {

SceneSelectCharacter::SceneSelectCharacter(const TestContext& ctx) : Test(ctx), gameManager_(*ctx.gameManager) {
  auto* gc = gameManager_.getCurrentCharacter();
  if (gc != nullptr) {
    currentTexture_ = std::make_unique<Texture>(gc->texturePath, false);
  }

  VertexBufferLayout verticesLayout;
  verticesLayout.pushFloat(2);  // Each vertex is made of 2 floats
  VertexBufferLayout texturesLayout;
  texturesLayout.pushFloat(2);  // Each texture coordinates is made of 2 floats
  va_->setBufferLayout({{*vbVertices_, verticesLayout}, {*vbTextures_, texturesLayout}});

  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto& aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  renderer_.enableBlend();
  defaultTexture_->bind(0);
  shader_->setUniform1i("u_textureSampler", 0);

  shader_->unBind();
  va_->unBind();
  vbVertices_->unBind();
}

SceneSelectCharacter::~SceneSelectCharacter() = default;

void SceneSelectCharacter::onUpdate(float /*deltaTime */) {}

void SceneSelectCharacter::onRender() {
  auto* gc = gameManager_.getCurrentCharacter();
  auto& gcTexture = gc == nullptr ? defaultTexture_ : currentTexture_;
  shader_->bind();
  gcTexture->bind(0);
  shader_->setUniform1i("u_textureSampler", 0);

  renderer_.clearColorBackground(backgroundColor_);
  renderer_.draw(*shader_, *va_, vertices_.size());
}

void SceneSelectCharacter::onImGuiRender() {
  ImGui::Text("Your character");
  ImGui::Indent();
  auto* gc = gameManager_.getCurrentCharacter();
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
      gameManager_.setCurrentCharacter(character);
      currentTexture_ = std::make_unique<Texture>(character.texturePath, false);
    }
  }
  ImGui::Unindent();

  ImGui::Text("Background color");
  ImGui::Indent();
  ImGui::ColorEdit4("", backgroundColor_.data());
  ImGui::Unindent();
}

}  // namespace test