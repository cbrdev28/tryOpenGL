#include "TestBatchRender.h"

#include <VertexBufferLayout.h>
#include <glmHeaders.h>
#include <imgui.h>
#include <openGLErrorHelpers.h>

#include <vector>

namespace test {

TestBatchRender::TestBatchRender(const TestContext& ctx) : Test(ctx) {
  std::vector<float> positions = {
      // clang-format off
    -0.75F, -0.25F, 0.0F, 0.0F,
    -0.25F, -0.25F, 1.0F, 0.0F,
    -0.25F, 0.25F, 1.0F, 1.0F,
    -0.75F, 0.25F, 0.0F, 1.0F,

    0.0F, -0.5F, 0.0F, 0.0F,
    1.0F, -0.5F, 1.0F, 0.0F,
    1.0F, 0.5F, 1.0F, 1.0F,
    0.0F, 0.5F, 0.0F, 1.0F
      // clang-format on
  };
  std::vector<unsigned int> indices = {
      // clang-format off
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4
      // clang-format on
  };

  va_ = std::make_unique<VertexArray>();
  vb_ = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));

  VertexBufferLayout layout;
  layout.pushFloat(2);
  layout.pushFloat(2);
  va_->addBuffer(*vb_, layout);

  ib_ = std::make_unique<IndexBuffer>(indices.data(), indices.size());

  shader_ = std::make_unique<ShaderManager>("../res/shaders/test_texture.shader");
  shader_->init();
  shader_->bind();
  glm::mat4 identityMatrix = glm::mat4{1.0F};
  shader_->setUniformMat4("u_model", identityMatrix);
  shader_->setUniformMat4("u_view", identityMatrix);
  const auto reversedAspectRatio = 1.0F / ctx.windowManager.getAspectRatio().ratio;
  const auto zoom = zoom_ * 0.1F;
  shader_->setUniformMat4("u_projection", glm::ortho(-1.0F * zoom, 1.0F * zoom, -reversedAspectRatio * zoom,
                                                     reversedAspectRatio * zoom, -1.0F, 1.0F));

  texture_ = std::make_unique<Texture>("../res/textures/wall_texture.png");
  texture_->bind(0);
  shader_->setUniform1i("u_textureSampler", 0);

  va_->unBind();
  vb_->unBind();
  ib_->unBind();
  shader_->unBind();
}

TestBatchRender::~TestBatchRender() = default;

void TestBatchRender::onUpdate(float deltaTime) {}

void TestBatchRender::onRender() {
  shader_->bind();
  const auto reversedAspectRatio = 1.0F / this->getTestContext().windowManager.getAspectRatio().ratio;
  const auto zoom = zoom_ * 0.1F;
  shader_->setUniformMat4("u_projection", glm::ortho(-1.0F * zoom, 1.0F * zoom, -reversedAspectRatio * zoom,
                                                     reversedAspectRatio * zoom, -1.0F, 1.0F));
  GLCall(glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]));
  renderer_.draw(*shader_, *va_, *ib_);
}

void TestBatchRender::onImGuiRender() {
  ImGui::Text("Window width: %d", this->getTestContext().windowManager.getWidth());
  ImGui::Text("Window height: %d", this->getTestContext().windowManager.getHeight());
  ImGui::Text("%s", this->getTestContext().windowManager.getAspectRatio().formattedValue().c_str());
  ImGui::ColorEdit4("Color", backgroundColor_.data());
  ImGui::SliderFloat("Zoom", &zoom_, 1.0F, 100.0F, "WIP value = %.2f");
}

}  // namespace test
