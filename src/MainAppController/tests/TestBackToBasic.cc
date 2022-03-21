#include "TestBackToBasic.h"

#include "MatrixHelper.h"
#include "VertexBufferElement.h"
#include "imgui.h"

namespace test {

TestBackToBasic::TestBackToBasic(const TestContext& ctx) : Test(ctx), instancedTriangle_() {
  // ib_ = std::make_unique<IndexBuffer>(instancedTriangle_.indices.data(), instancedTriangle_.indices.size());
  va_ = std::make_unique<VertexArray>();
  vbModelVertex0_ =
      std::make_unique<VertexBuffer>(instancedTriangle_.vertices.data(), instancedTriangle_.verticesGLSize());
  vbModelPosition1_ = std::make_unique<VertexBuffer>(nullptr, instancedTriangle_.maxPositionsGLSize(), GL_STREAM_DRAW);
  // For now, both of our vertex buffer share the same layout element: 2 floats (normalized = false)
  std::vector<std::pair<const VertexBuffer&, const VertexBufferElement&>> buffersAndElements = {
      {*vbModelVertex0_, {GL_FLOAT, 2, GL_FALSE}},
      {*vbModelPosition1_, {GL_FLOAT, 2, GL_FALSE}},
  };
  va_->setInstanceBufferElement(buffersAndElements);

  shader_ = std::make_unique<ShaderManager>("test_back_to_basic.shader");
  shader_->bind();
  shader_->setUniformMat4("u_model", MatrixHelper::identityMatrix);
  shader_->setUniformMat4("u_view", MatrixHelper::identityMatrix);

  const auto reversedAspectRation = ctx.windowManager->getAspectRatio().reversed();
  shader_->setUniformMat4("u_projection",
                          glm::ortho(-1.0F, 1.0F, -reversedAspectRation, reversedAspectRation, -1.0F, 1.0F));

  va_->unBind();
  vbModelPosition1_->unBind();
  vbModelVertex0_->unBind();
  // ib_->unBind();
  shader_->unBind();
}

TestBackToBasic::~TestBackToBasic() = default;

void TestBackToBasic::onUpdate(float deltaTime) {}

void TestBackToBasic::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  // renderer_.draw(*shader_, *va_, *ib_);
  // TODO(cbr): make dynamic draw
  // renderer_.drawIndexInstance(*shader_, *va_, *ib_, static_cast<GLsizei>(instancedTriangle_.positions.size()));
  renderer_.drawInstance(*shader_, *va_, 3, static_cast<GLsizei>(instancedTriangle_.positions.size()));
}

void TestBackToBasic::onImGuiRender() {
  ImGui::ColorEdit4("Color", backgroundColor_.data());
  if (ImGui::Button("Add instance")) {
    this->addTriangleInstance();
  }
  ImGui::Text("Positions count: %.zu", instancedTriangle_.positions.size());
}

void TestBackToBasic::addTriangleInstance() {
  // Make
  const auto positions = instancedTriangle_.addTriangle();
  // Add
  vbModelPosition1_->setInstanceData(positions.data(), instancedTriangle_.positionsGLSize(),
                                     instancedTriangle_.maxPositionsGLSize());
  vbModelPosition1_->unBind();
}

}  // namespace test