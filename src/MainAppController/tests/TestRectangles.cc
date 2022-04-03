#include "TestRectangles.h"

#include <imgui.h>

#include <chrono>

#include "VertexBufferLayout.h"

namespace test {

TestRectangles::TestRectangles(const TestContext& ctx) : Test(ctx) {
  ctx.windowManager->addWindowListener(this);

  smallRectPositions_.reserve(TestRectangles::kMaxSmallRect);
  smallRectAngles_.reserve(TestRectangles::kMaxSmallRect);
  mediumRectPositions_.reserve(TestRectangles::kMaxMediumRect);
  mediumRectAngles_.reserve(TestRectangles::kMaxMediumRect);

  vbRectVertices_ = std::make_unique<VertexBuffer>(rectVertices.data(), sizeof(rectVertices));
  vbRectPositions_ = std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * kMaxRect, GL_STREAM_DRAW);
  vbRectAngles_ = std::make_unique<VertexBuffer>(nullptr, sizeof(GLfloat) * kMaxRect, GL_STREAM_DRAW);

  vbRectVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbRectPositions_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbRectAngles_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout rectVerticesLayout;
  rectVerticesLayout.pushFloat(2);  // Each vertex is made of 2 floats
  VertexBufferLayout rectPositionsLayout;
  rectPositionsLayout.pushFloat(2);  // Each position is made of 2 floats
  VertexBufferLayout rectAnglesLayout;
  rectPositionsLayout.pushFloat(1);  // An angle is a single float

  vaRect_->setInstanceBufferLayout({
      {*vbRectVertices_, rectVerticesLayout},
      {*vbRectPositions_, rectPositionsLayout},
      {*vbRectAngles_, rectAnglesLayout},
  });

  shaderRect_->bind();
  shaderRect_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shaderRect_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  vaRect_->unBind();
  vbRectAngles_->unBind();
  vbRectPositions_->unBind();
  vbRectVertices_->unBind();
  shaderRect_->unBind();
}

TestRectangles::~TestRectangles() { this->getTestContext().windowManager->removeWindowListener(this); }

void TestRectangles::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;

  if (!useThreads_) {
    // Some update in the main thread
  } else {
    onThreadedUpdate(deltaTime);
  }
}

void TestRectangles::onRender() {
  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  renderer_.drawInstance(*shaderRect_, *vaRect_, static_cast<GLsizei>(rectVertices.size()),
                         static_cast<GLsizei>(smallRectPositions_.size() + mediumRectPositions_.size()));
}

void TestRectangles::onImGuiRender() {
  ImGui::NewLine();
  ImGui::Text("FPS: %.2f", 1.0F / deltaTime_);
  auto frameCount = this->getTestContext().windowManager->getWindowStats().frameCount;
  auto startTime = this->getTestContext().windowManager->getWindowStats().startTime;
  auto endTime = this->getTestContext().windowManager->getWindowStats().endTime;
  ImGui::Text("Avg FPS: %.2f",
              static_cast<float>(frameCount) /
                  std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count());
  ImGui::Text("Avg rendering: %.2f ms",
              std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(endTime - startTime).count() /
                  static_cast<float>(frameCount));

  ImGui::NewLine();
  ImGui::Text("Width: %d", this->getTestContext().windowManager->getWidth());
  ImGui::Text("Height: %d", this->getTestContext().windowManager->getHeight());
  ImGui::ColorEdit4("Color", backgroundColor_.data());

  ImGui::NewLine();
  ImGui::Checkbox("Use threads", &useThreads_);
}

void TestRectangles::onKeyADown() {}

void TestRectangles::onKeyWDown() {}

void TestRectangles::onKeySDown() {}

void TestRectangles::onKeyDDown() {}

void TestRectangles::onThreadedUpdate(float dt) {
  // auto threadPool = this->getTestContext().threadPoolManager;
  // Do something with threads
}

}  // namespace test