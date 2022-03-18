#include "TestBackToBasic.h"

#include "VertexBufferLayout.h"
#include "glmHeaders.h"
#include "imgui.h"
#include "openGLErrorHelpers.h"

namespace test {

TestBackToBasic::TestBackToBasic(const TestContext& ctx) : Test(ctx) {
  vb_ = std::make_unique<VertexBuffer>(vertices_.data(), vertices_.size() * sizeof(float));

  va_ = std::make_unique<VertexArray>();
  VertexBufferLayout layout;
  layout.pushFloat(2);
  layout.pushFloat(2);
  va_->addBuffer(*vb_, layout);

  ib_ = std::make_unique<IndexBuffer>(indices_.data(), indices_.size());

  shader_ = std::make_unique<ShaderManager>("test_back_to_basic.shader");
  shader_->init();
  shader_->bind();
  glm::mat4 identityMatrix = glm::mat4{1.0F};
  shader_->setUniformMat4("u_model", identityMatrix);
  shader_->setUniformMat4("u_view", identityMatrix);
  shader_->setUniformMat4("u_projection", identityMatrix);

  texture_ = std::make_unique<Texture>("grass.png");
  texture_->bind(0);
  shader_->setUniform1i("u_textureSampler", 0);

  va_->unBind();
  vb_->unBind();
  ib_->unBind();
  shader_->unBind();
}

TestBackToBasic::~TestBackToBasic() = default;

void TestBackToBasic::onUpdate(float deltaTime) {}
void TestBackToBasic::onRender() {
  GLCall(glClearColor(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2), backgroundColor_.at(3)));
  renderer_.draw(*shader_, *va_, *ib_);
}
void TestBackToBasic::onImGuiRender() { ImGui::ColorEdit4("Color", backgroundColor_.data()); }

}  // namespace test