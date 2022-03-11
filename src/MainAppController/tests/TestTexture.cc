#include "TestTexture.h"

#include <imgui.h>
#include <openGLErrorHelpers.h>

#include <vector>

#include "VertexBufferLayout.h"
#include "glm/fwd.hpp"

namespace test {

TestTexture::TestTexture(const TestContext& ctx) : Test(ctx) {
  std::vector<float> positions = {
      // clang-format off
    -0.5F, -0.5F,
    0.5F, -0.5F,
    0.5F, 0.5F,
    -0.5F, 0.5F,
      // clang-format on
  };
  std::vector<unsigned int> indices = {
      // clang-format off
    0, 1, 2,
    2, 3, 0
      // clang-format on
  };

  va_ = std::make_unique<VertexArray>();
  vb_ = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(2);
  va_->addBuffer(*vb_, layout);

  ib_ = std::make_unique<IndexBuffer>(indices.data(), indices.size());

  shader_ = std::make_unique<ShaderManager>("../res/shaders/test_texture.shader");
  shader_->init();
  shader_->bind();
  glm::mat4 identityMatrix = glm::mat4{1.0F};
  shader_->setModelMatrix(identityMatrix).setViewMatrix(identityMatrix).setProjectionMatrix(identityMatrix);

  // Maybe more

  va_->unBind();
  vb_->unBind();
  ib_->unBind();
  shader_->unBind();
}

TestTexture::~TestTexture() = default;

void TestTexture::onUpdate(float deltaTime) {}
void TestTexture::onRender() {
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader_, *va_, *ib_);
}
void TestTexture::onImGuiRender() { ImGui::ColorEdit4("Color", backgroundColor_.data()); }

}  // namespace test