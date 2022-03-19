#include "TestBackToBasic.h"

#include "MatrixHelper.h"
#include "VertexBufferLayout.h"
#include "imgui.h"
#include "openGLErrorHelpers.h"

namespace test {

TestBackToBasic::TestBackToBasic(const TestContext& ctx) : Test(ctx) {
  vb_ = std::make_unique<VertexBuffer>(vertices_.data(), vertices_.size() * sizeof(float));
  ib_ = std::make_unique<IndexBuffer>(indices_.data(), indices_.size());
  va_ = std::make_unique<VertexArray>();

  VertexBufferLayout layout;
  layout.pushFloat(2);
  va_->addBuffer(*vb_, layout);

  shader_ = std::make_unique<ShaderManager>("test_back_to_basic.shader");
  shader_->init();
  shader_->bind();
  shader_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  shader_->setUniformMat4("u_view", MatrixHelper::identityMatrix);
  // Update to ortho
  shader_->setUniformMat4("u_projection", MatrixHelper::identityMatrix);

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