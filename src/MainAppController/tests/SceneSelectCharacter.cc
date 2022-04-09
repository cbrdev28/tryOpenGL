#include "cbrpch.h"

#include "SceneSelectCharacter.h"

#include "VertexBufferLayout.h"

namespace test {

SceneSelectCharacter::SceneSelectCharacter(const TestContext& ctx) : Test(ctx) {
  vbVertices_ = std::make_unique<VertexBuffer>(vertices_.data(), sizeof(vertices_));
  vbTextures_ = std::make_unique<VertexBuffer>(textures_.data(), sizeof(textures_));

  VertexBufferLayout verticesLayout;
  verticesLayout.pushFloat(2);  // Each vertex is made of 2 floats
  VertexBufferLayout texturesLayout;
  texturesLayout.pushFloat(2);  // Each texture coordinates is made of 2 floats
  va_->setBufferLayout({{*vbVertices_, verticesLayout}, {*vbTextures_, texturesLayout}});

  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto& aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  texture_->bind(0);
  shader_->setUniform1i("u_textureSampler", 0);

  // texture_->unBind();
  shader_->unBind();
  va_->unBind();
  vbVertices_->unBind();
}

SceneSelectCharacter::~SceneSelectCharacter() = default;

void SceneSelectCharacter::onUpdate(float /*deltaTime */) {}

void SceneSelectCharacter::onRender() {
  renderer_.clearColorBackground(backgroundColor_);
  renderer_.draw(*shader_, *va_, vertices_.size());
}

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