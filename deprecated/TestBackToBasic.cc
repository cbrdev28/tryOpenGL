#include "TestBackToBasic.h"

#include "MatrixHelper.h"
#include "VertexBufferElement.h"
#include "VertexBufferLayout.h"
#include "imgui.h"

namespace test {

TestBackToBasic::TestBackToBasic(const TestContext& ctx) : Test(ctx), instancedTriangle_() {
  va_ = std::make_unique<VertexArray>();
  vbModelVertex0_ =
      std::make_unique<VertexBuffer>(instancedTriangle_.vertices.data(), instancedTriangle_.verticesGLSize());
  vbModelTransformation2_ =
      std::make_unique<VertexBuffer>(nullptr, instancedTriangle_.maxTransformationsGLSize(), GL_STREAM_DRAW);
  vbModelVertex0_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbModelTransformation2_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout layoutModel;
  layoutModel.pushFloat(2);
  VertexBufferLayout layoutPosition;
  layoutPosition.pushFloat(2);
  VertexBufferLayout layoutTransformation;
  layoutTransformation.pushFloat(4);
  layoutTransformation.pushFloat(4);
  layoutTransformation.pushFloat(4);
  layoutTransformation.pushFloat(4);
  const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>> vectorOfPairs = {
      {*vbModelVertex0_, layoutModel},
      {*vbModelTransformation2_, layoutTransformation},
  };
  va_->setInstanceBufferLayout(vectorOfPairs);

  shader_ = std::make_unique<ShaderManager>("test_back_to_basic.shader");
  shader_->bind();
  shader_->setUniformMat4("u_view", MatrixHelper::identityMatrix);

  const auto aspectRatio = ctx.windowManager->getAspectRatio().ratio;
  shader_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  va_->unBind();
  vbModelTransformation2_->unBind();
  vbModelVertex0_->unBind();
  shader_->unBind();
}

TestBackToBasic::~TestBackToBasic() = default;

void TestBackToBasic::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;
  instancedTriangle_.updateTransformation(deltaTime);
  vbModelTransformation2_->setInstanceData(instancedTriangle_.transformations.data(),
                                           instancedTriangle_.transformationsGLSize(),
                                           instancedTriangle_.maxTransformationsGLSize());
  vbModelTransformation2_->unBind();
}

void TestBackToBasic::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  renderer_.drawInstance(*shader_, *va_, static_cast<GLsizei>(instancedTriangle_.vertices.size()),
                         static_cast<GLsizei>(instancedTriangle_.positions.size()));
}

void TestBackToBasic::onImGuiRender() {
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  ImGui::ColorEdit4("Color", backgroundColor_.data());
  if (ImGui::Button("Add instance")) {
    this->addTriangleInstance();
  }
  ImGui::Text("Positions count: %.zu", instancedTriangle_.positions.size());
  ImGui::Text("Transformations count: %.zu", instancedTriangle_.transformations.size());
}

void TestBackToBasic::addTriangleInstance() {
  instancedTriangle_.addTriangle();

  vbModelTransformation2_->setInstanceData(instancedTriangle_.transformations.data(),
                                           instancedTriangle_.transformationsGLSize(),
                                           instancedTriangle_.maxTransformationsGLSize());
  vbModelTransformation2_->unBind();
}

}  // namespace test