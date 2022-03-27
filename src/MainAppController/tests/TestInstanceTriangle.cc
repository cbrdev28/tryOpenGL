#include "TestInstanceTriangle.h"

#include "VertexBufferLayout.h"
#include "imgui.h"

namespace test {

TestInstanceTriangle::TestInstanceTriangle(const TestContext& ctx) : Test(ctx) {
  va_ = std::make_unique<VertexArray>();
  vbModelVertex0_ =
      std::make_unique<VertexBuffer>(instancedTriangle_.vertices.data(), instancedTriangle_.verticesGLSize());
  vbModelPositions2_ = std::make_unique<VertexBuffer>(nullptr, instancedTriangle_.maxPositionsGLSize(), GL_STREAM_DRAW);
  vbModelVertex0_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbModelPositions2_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout layoutModel;
  layoutModel.pushFloat(2);
  VertexBufferLayout layoutPosition;
  layoutPosition.pushFloat(2);
  const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>> vectorOfPairs = {
      {*vbModelVertex0_, layoutModel},
      {*vbModelPositions2_, layoutPosition},
  };
  va_->setInstanceBufferLayout(vectorOfPairs);

  shader_ = std::make_unique<Shader>("test_instance_triangle.shader");
  shader_->bind();
  shader_->setUniformMat4("u_view", glm::mat4(1.0F));

  const auto aspectRatio = ctx.windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  va_->unBind();
  vbModelPositions2_->unBind();
  vbModelVertex0_->unBind();
  shader_->unBind();
}

TestInstanceTriangle::~TestInstanceTriangle() = default;

void TestInstanceTriangle::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;
  // TODO(cbr): update rotation angle
}

void TestInstanceTriangle::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  renderer_.drawInstance(*shader_, *va_, static_cast<GLsizei>(instancedTriangle_.vertices.size()),
                         static_cast<GLsizei>(instancedTriangle_.positions.size()));
}

void TestInstanceTriangle::onImGuiRender() {
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  ImGui::Text("Width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Height: %d", this->getTestContext().windowManager->getHeight());
  ImGui::ColorEdit4("Color", backgroundColor_.data());
  if (ImGui::Button("Add instance")) {
    this->addTriangleInstance();
  }
  ImGui::Text("Positions count: %.zu", instancedTriangle_.positions.size());
}

void TestInstanceTriangle::addTriangleInstance() {
  instancedTriangle_.addTriangle();

  vbModelPositions2_->setInstanceData(instancedTriangle_.positions.data(), instancedTriangle_.positionsGLSize(),
                                      instancedTriangle_.maxPositionsGLSize());
  vbModelPositions2_->unBind();
}

}  // namespace test