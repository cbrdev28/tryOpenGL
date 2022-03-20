#include "TestTexture.h"

#include <imgui.h>

#include <array>

#include "VertexBufferLayout.h"
#include "glmHeaders.h"

namespace test {

TestTexture::TestTexture(const TestContext& ctx) : Test(ctx) {
  std::array<float, 16> positions = {
      // clang-format off
    -0.5F, -0.5F, 0.0F, 0.0F,
    0.5F, -0.5F, 1.0F, 0.0F,
    0.5F, 0.5F, 1.0F, 1.0F,
    -0.5F, 0.5F, 0.0F, 1.0F
      // clang-format on
  };
  std::array<unsigned int, 6> indices = {
      // clang-format off
    0, 1, 2,
    2, 3, 0
      // clang-format on
  };

  va_ = std::make_unique<VertexArray>();
  vb_ = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(2, *vb_);
  layout.pushFloat(2, *vb_);
  va_->setBufferLayout(layout);

  ib_ = std::make_unique<IndexBuffer>(indices.data(), indices.size());

  shader_ = std::make_unique<ShaderManager>("test_texture.shader");
  shader_->bind();
  glm::mat4 identityMatrix = glm::mat4{1.0F};
  shader_->setUniformMat4("u_model", identityMatrix);
  shader_->setUniformMat4("u_view", identityMatrix);
  shader_->setUniformMat4("u_projection", identityMatrix);

  texture_ = std::make_unique<Texture>("wall.png");
  texture_->bind(0);
  shader_->setUniform1i("u_textureSampler", 0);

  va_->unBind();
  vb_->unBind();
  ib_->unBind();
  shader_->unBind();
}

TestTexture::~TestTexture() = default;

void TestTexture::onUpdate(float deltaTime) {}
void TestTexture::onRender() {
  renderer_.clearColorBackground(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);
  renderer_.draw(*shader_, *va_, *ib_);
}
void TestTexture::onImGuiRender() { ImGui::ColorEdit4("Color", backgroundColor_.data()); }

}  // namespace test