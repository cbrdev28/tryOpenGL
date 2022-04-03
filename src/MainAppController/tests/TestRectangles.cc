#include "TestRectangles.h"

#include <imgui.h>

#include <chrono>

#include "VertexBufferLayout.h"
#include "openGLErrorHelpers.h"

namespace test {

TestRectangles::TestRectangles(const TestContext& ctx) : Test(ctx) {
  ctx.windowManager->addWindowListener(this);

  smallRectPositions_.reserve(TestRectangles::kMaxSmallRect);
  smallRectAngles_.reserve(TestRectangles::kMaxSmallRect);
  smallRectScales_.reserve(TestRectangles::kMaxSmallRect);
  mediumRectPositions_.reserve(TestRectangles::kMaxMediumRect);
  mediumRectAngles_.reserve(TestRectangles::kMaxMediumRect);
  mediumRectScales_.reserve(TestRectangles::kMaxMediumRect);

  mainSquaresPositions_.reserve(TestRectangles::kMaxMainSquares);
  mainSquaresAngles_.reserve(TestRectangles::kMaxMainSquares);
  mainSquaresScales_.reserve(TestRectangles::kMaxMainSquares);
  mainSquaresPositions_.emplace_back(0.0F, 0.0F);
  mainSquaresAngles_.emplace_back(glm::radians(0.0F));
  mainSquaresScales_.emplace_back(1.0F, 1.0F);

  vbRectVertices_ = std::make_unique<VertexBuffer>(rectVertices.data(), sizeof(rectVertices));
  vbRectPositions_ = std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * kMaxRect, GL_STREAM_DRAW);
  vbRectAngles_ = std::make_unique<VertexBuffer>(nullptr, sizeof(GLfloat) * kMaxRect, GL_STREAM_DRAW);
  vbRectScales_ = std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec2) * kMaxRect, GL_STREAM_DRAW);

  vbRectVertices_->setDivisor(VertexBufferDivisor::ALWAYS);
  vbRectPositions_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbRectAngles_->setDivisor(VertexBufferDivisor::FOR_EACH);
  vbRectScales_->setDivisor(VertexBufferDivisor::FOR_EACH);

  VertexBufferLayout rectVerticesLayout;
  rectVerticesLayout.pushFloat(2);  // Each vertex is made of 2 floats
  VertexBufferLayout rectPositionsLayout;
  rectPositionsLayout.pushFloat(2);  // Each position is made of 2 floats
  VertexBufferLayout rectAnglesLayout;
  rectAnglesLayout.pushFloat(1);  // An angle is a single float
  VertexBufferLayout rectScalesLayout;
  rectScalesLayout.pushFloat(2);  // Each scale is made of 2 floats

  vaRect_->setInstanceBufferLayout({
      {*vbRectVertices_, rectVerticesLayout},
      {*vbRectPositions_, rectPositionsLayout},
      {*vbRectAngles_, rectAnglesLayout},
      {*vbRectScales_, rectScalesLayout},
  });

  shaderRect_->bind();
  shaderRect_->setUniformMat4("u_view", glm::mat4(1.0F));
  const auto aspectRatio = this->getTestContext().windowManager->getAspectRatio().ratio;
  shaderRect_->setUniformMat4("u_projection", glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F));

  vaRect_->unBind();
  vbRectScales_->unBind();
  vbRectAngles_->unBind();
  vbRectPositions_->unBind();
  vbRectVertices_->unBind();
  shaderRect_->unBind();

  this->setVBPositions();
  this->setVBAngles();
  this->setVBScales();
}

TestRectangles::~TestRectangles() { this->getTestContext().windowManager->removeWindowListener(this); }

void TestRectangles::onUpdate(float deltaTime) {
  deltaTime_ = deltaTime;

  if (useThreads_) {
    onThreadedUpdate(deltaTime);
  } else {
    for (auto& smallRectAngle : smallRectAngles_) {
      smallRectAngle += TestRectangles::kRotationAngle * TestRectangles::kRotationSpeed * deltaTime;
    }
    for (auto& mediumRectAngle : mediumRectAngles_) {
      mediumRectAngle -= TestRectangles::kRotationAngle * TestRectangles::kRotationSpeed * deltaTime;
    }
    this->setVBAngles();
  }
}

void TestRectangles::onRender() {
  if (usePolygons_) {
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
  } else {
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  }

  renderer_.clearColorBackground(backgroundColor_.at(0), backgroundColor_.at(1), backgroundColor_.at(2),
                                 backgroundColor_.at(3));
  renderer_.drawInstance(*shaderRect_, *vaRect_, static_cast<GLsizei>(rectVertices.size()), this->currentRectCount());
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
  ImGui::Checkbox("Set polygon", &usePolygons_);

  ImGui::NewLine();
  ImGui::Text("Small pos: %zu", smallRectPositions_.size());
  ImGui::Text("Medium pos: %zu", mediumRectPositions_.size());
  ImGui::Text("Total pos: %d", this->currentRectCount());
  if (ImGui::Button("Spawn small")) {
    this->spawnReact(true);
  }
  if (ImGui::Button("Spawn medium")) {
    this->spawnReact(false);
  }
}

void TestRectangles::onKeyADown() {}

void TestRectangles::onKeyWDown() {}

void TestRectangles::onKeySDown() {}

void TestRectangles::onKeyDDown() {}

void TestRectangles::onThreadedUpdate(float dt) {
  // auto threadPool = this->getTestContext().threadPoolManager;
  // Do something with threads
}

void TestRectangles::spawnReact(const bool& small, const glm::vec2& target) {
  ASSERT(this->currentRectCount() < TestRectangles::kMaxRect);
  // Random positions between: -0.9 & 0.9
  const auto expectedValueRange = 0.9F;
  auto randomPosX = expectedValueRange - (2 * this->genRandom() * expectedValueRange);
  auto randomPosY = expectedValueRange - (2 * this->genRandom() * expectedValueRange);

  // If the triangle is within "a close" range from the target, push it away
  const float rangeOffset = 0.5F;
  glm::vec2 deltaTriangleTarget = {randomPosX - target.x, randomPosY - target.y};
  if (glm::abs(deltaTriangleTarget.x) < rangeOffset && glm::abs(deltaTriangleTarget.y) < rangeOffset) {
    float xMultiplier = deltaTriangleTarget.x > 0 ? 1.0F : -1.0F;
    float yMultiplier = deltaTriangleTarget.y > 0 ? 1.0F : -1.0F;
    float xOffset = rangeOffset - glm::abs(deltaTriangleTarget.x);
    float yOffset = rangeOffset - glm::abs(deltaTriangleTarget.y);

    randomPosX += xOffset * xMultiplier;
    randomPosY += yOffset * yMultiplier;
  }

  if (small) {
    ASSERT(smallRectPositions_.size() < TestRectangles::kMaxSmallRect);
    smallRectPositions_.emplace_back(randomPosX, randomPosY);
    smallRectAngles_.emplace_back(0.0F);
    smallRectScales_.emplace_back(1.0F, 2.0F);
  } else {
    ASSERT(mediumRectPositions_.size() < TestRectangles::kMaxMediumRect);
    mediumRectPositions_.emplace_back(randomPosX, randomPosY);
    mediumRectAngles_.emplace_back(0.0F);
    mediumRectScales_.emplace_back(1.5F, 3.0F);
  }

  this->setVBPositions();
  this->setVBAngles();
  this->setVBScales();
}

void TestRectangles::setVBPositions() {
  vbRectPositions_->setInstanceData(smallRectPositions_.data(),
                                    static_cast<GLsizeiptr>(sizeof(glm::vec2) * smallRectPositions_.size()),
                                    sizeof(glm::vec2) * kMaxRect);
  vbRectPositions_->setInstanceDataOffset(mediumRectPositions_.data(),
                                          static_cast<GLsizeiptr>(sizeof(glm::vec2) * mediumRectPositions_.size()),
                                          static_cast<GLintptr>(sizeof(glm::vec2) * smallRectPositions_.size()));
  vbRectPositions_->setInstanceDataOffset(mainSquaresPositions_.data(),
                                          static_cast<GLsizeiptr>(sizeof(glm::vec2) * mainSquaresPositions_.size()),
                                          static_cast<GLintptr>(sizeof(glm::vec2) * smallRectPositions_.size()) +
                                              static_cast<GLintptr>(sizeof(glm::vec2) * mediumRectPositions_.size()));
  vbRectPositions_->unBind();
}

void TestRectangles::setVBAngles() {
  vbRectAngles_->setInstanceData(smallRectAngles_.data(),
                                 static_cast<GLsizeiptr>(sizeof(GLfloat) * smallRectAngles_.size()),
                                 sizeof(GLfloat) * kMaxRect);
  vbRectAngles_->setInstanceDataOffset(mediumRectAngles_.data(),
                                       static_cast<GLsizeiptr>(sizeof(GLfloat) * mediumRectAngles_.size()),
                                       static_cast<GLintptr>(sizeof(GLfloat) * smallRectAngles_.size()));
  vbRectAngles_->setInstanceDataOffset(mainSquaresAngles_.data(),
                                       static_cast<GLsizeiptr>(sizeof(GLfloat) * mainSquaresAngles_.size()),
                                       static_cast<GLintptr>(sizeof(GLfloat) * smallRectAngles_.size()) +
                                           static_cast<GLintptr>(sizeof(GLfloat) * mediumRectAngles_.size()));
  vbRectAngles_->unBind();
}

void TestRectangles::setVBScales() {
  vbRectScales_->setInstanceData(smallRectScales_.data(),
                                 static_cast<GLsizeiptr>(sizeof(glm::vec2) * smallRectScales_.size()),
                                 sizeof(glm::vec2) * kMaxRect);
  vbRectScales_->setInstanceDataOffset(mediumRectScales_.data(),
                                       static_cast<GLsizeiptr>(sizeof(glm::vec2) * mediumRectScales_.size()),
                                       static_cast<GLintptr>(sizeof(glm::vec2) * smallRectScales_.size()));
  vbRectScales_->setInstanceDataOffset(mainSquaresScales_.data(),
                                       static_cast<GLsizeiptr>(sizeof(glm::vec2) * mainSquaresScales_.size()),
                                       static_cast<GLintptr>(sizeof(glm::vec2) * smallRectScales_.size()) +
                                           static_cast<GLintptr>(sizeof(glm::vec2) * mediumRectScales_.size()));
  vbRectScales_->unBind();
}

}  // namespace test