#include "TestBatchRender.h"

#include <imgui.h>

#include <array>

#include "VertexBufferLayout.h"
#include "glmHeaders.h"

namespace test {

TestBatchRender::TestBatchRender(const TestContext& ctx) : Test(ctx) {
  std::array<float, 40> positions = {
      // clang-format off
    // 2 coords, 2 texture coord, 1 texture id
    -0.75F, -0.25F,  0.0F, 0.0F,  0.0F,
    -0.25F, -0.25F,  1.0F, 0.0F,  0.0F,
    -0.25F, 0.25F,  1.0F, 1.0F,  0.0F,
    -0.75F, 0.25F,  0.0F, 1.0F,  0.0F,

    0.0F, -0.5F,  0.0F, 0.0F,  1.0F,
    1.0F, -0.5F,  1.0F, 0.0F,  1.0F,
    1.0F, 0.5F,  1.0F, 1.0F,  1.0F,
    0.0F, 0.5F,  0.0F, 1.0F,  1.0F
      // clang-format on
  };
  std::array<unsigned int, 12> indices = {
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
  layout.pushFloat(1);
  va_->addBuffer(*vb_, layout);

  ib_ = std::make_unique<IndexBuffer>(indices.data(), indices.size());

  shader_ = std::make_unique<ShaderManager>("test_batch_render.shader");
  shader_->bind();
  glm::mat4 identityMatrix = glm::mat4{1.0F};
  shader_->setUniformMat4("u_model", identityMatrix);
  this->setViewProjection(usePerspective_);

  textureGrass_ = std::make_unique<Texture>("grass.png");
  textureWall_ = std::make_unique<Texture>("wall.png");
  textureWall_->bind(0);
  textureGrass_->bind(1);
  // Set an array of samplers in our shader with values: 0, 1 (respectively matching the texture bind(...) function)
  shader_->setUniform1iv("u_textureSamplers", {0, 1});

  va_->unBind();
  vb_->unBind();
  ib_->unBind();
  shader_->unBind();
}

TestBatchRender::~TestBatchRender() = default;

void TestBatchRender::onUpdate(float deltaTime) {}

void TestBatchRender::onRender() {
  this->setViewProjection(usePerspective_);
  renderer_.clearColorBackground(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);
  renderer_.draw(*shader_, *va_, *ib_);
}

void TestBatchRender::onImGuiRender() {
  ImGui::Text("Window width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Window height: %d", this->getTestContext().windowManager->getHeight());
  ImGui::Text("%s", this->getTestContext().windowManager->getAspectRatio().formattedValue().c_str());
  ImGui::ColorEdit4("Color", backgroundColor_.data());
  ImGui::Checkbox("Use perspective", &usePerspective_);
  if (usePerspective_) {
    ImGui::SliderFloat("FOV", &fov_, 1.0F, 180.0F, "WIP value = %.2f");
  } else {
    ImGui::SliderFloat("Zoom", &zoom_, 1.0F, 100.0F, "WIP value = %.2f");
  }
  ImGui::SliderFloat("Delta X", &deltaX_, -100.0F, 100.0F, "WIP value = %.2f");
  ImGui::SliderFloat("Delta Y", &deltaY_, -100.0F, 100.0F, "WIP value = %.2f");
}

void TestBatchRender::setViewProjection(bool usePerspective) {
  shader_->bind();
  if (usePerspective) {
    const auto deltaX = deltaX_ * 0.1F;
    const auto deltaY = deltaY_ * 0.1F;
    glm::vec3 pos = {0.0F + deltaX, 0.0F + deltaY, 0.0F};
    glm::vec3 target = {0.0F, 0.0F, -1.0F};
    glm::vec3 up = {0.0F, 1.0F, 0.0F};
    // Move camera: up on Z axis & back on the Y axis (to look from above and a bit behind)
    glm::vec3 posOffset = {0.0F, -1.0F, 1.0F};
    shader_->setUniformMat4("u_view", glm::lookAt(pos + posOffset, target + pos, up));

    const auto perspective =
        glm::perspective(glm::radians(fov_), this->getTestContext().windowManager->getAspectRatio().ratio, 0.0F, 10.0F);
    shader_->setUniformMat4("u_projection", perspective);
  } else {
    shader_->setUniformMat4("u_view", glm::mat4{1.0F});

    const auto reversedAspectRatio = 1.0F / this->getTestContext().windowManager->getAspectRatio().ratio;
    const auto zoom = zoom_ * 0.1F;
    const auto deltaX = deltaX_ * 0.1F;
    const auto deltaY = deltaY_ * 0.1F;
    const auto orthoX = 1.0F * zoom;
    const auto orthoY = reversedAspectRatio * zoom;
    shader_->setUniformMat4("u_projection", glm::ortho((-orthoX) + deltaX, orthoX + deltaX, (-orthoY) + deltaY,
                                                       orthoY + deltaY, -1.0F, 1.0F));
  }
}

}  // namespace test
