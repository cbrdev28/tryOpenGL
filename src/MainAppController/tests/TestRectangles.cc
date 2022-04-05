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

  tinySquaresPositions_.reserve(TestRectangles::kMaxTinySquares);
  tinySquaresAngles_.reserve(TestRectangles::kMaxTinySquares);
  tinySquaresScales_.reserve(TestRectangles::kMaxTinySquares);

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
    // Main thread updates
    this->moveRectsToMainSquare();
    this->moveTinySquares();
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
    this->spawnReact(true, mainSquaresPositions_.at(0));
  }
  if (ImGui::Button("Spawn medium")) {
    this->spawnReact(false, mainSquaresPositions_.at(0));
  }

  ImGui::NewLine();
  ImGui::Text("Tiny count: %zu", tinySquaresPositions_.size());
  if (ImGui::Button("Spawn tiny")) {
    this->spawnTinySquares();
  }
}

void TestRectangles::onKeyADown() {
  mainSquaresPositions_.at(0).x -= 1.0F * TestRectangles::kMainSquareMoveSpeed * deltaTime_;
  this->setVBPositions();
  this->mainSquareCollisionRect();
}

void TestRectangles::onKeyWDown() {
  mainSquaresPositions_.at(0).y += 1.0F * TestRectangles::kMainSquareMoveSpeed * deltaTime_;
  this->setVBPositions();
  this->mainSquareCollisionRect();
}

void TestRectangles::onKeySDown() {
  mainSquaresPositions_.at(0).y -= 1.0F * TestRectangles::kMainSquareMoveSpeed * deltaTime_;
  this->setVBPositions();
  this->mainSquareCollisionRect();
}

void TestRectangles::onKeyDDown() {
  mainSquaresPositions_.at(0).x += 1.0F * TestRectangles::kMainSquareMoveSpeed * deltaTime_;
  this->setVBPositions();
  this->mainSquareCollisionRect();
}

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
    mediumRectScales_.emplace_back(3.0F, 1.5F);
  }

  this->setVBPositions();
  this->setVBAngles();
  this->setVBScales();
}

void TestRectangles::setVBPositions() {
  GLintptr offset = 0;
  auto sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * smallRectPositions_.size());
  vbRectPositions_->setInstanceData(smallRectPositions_.data(), sizeToSend, sizeof(glm::vec2) * kMaxRect);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * mediumRectPositions_.size());
  vbRectPositions_->setInstanceDataOffset(mediumRectPositions_.data(), sizeToSend, offset);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * mainSquaresPositions_.size());
  vbRectPositions_->setInstanceDataOffset(mainSquaresPositions_.data(), sizeToSend, offset);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * tinySquaresPositions_.size());
  vbRectPositions_->setInstanceDataOffset(tinySquaresPositions_.data(), sizeToSend, offset);
  vbRectPositions_->unBind();
}

void TestRectangles::setVBAngles() {
  GLintptr offset = 0;
  auto sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * smallRectAngles_.size());
  vbRectAngles_->setInstanceData(smallRectAngles_.data(), sizeToSend, sizeof(GLfloat) * kMaxRect);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * mediumRectAngles_.size());
  vbRectAngles_->setInstanceDataOffset(mediumRectAngles_.data(), sizeToSend, offset);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * mainSquaresAngles_.size());
  vbRectAngles_->setInstanceDataOffset(mainSquaresAngles_.data(), sizeToSend, offset);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(GLfloat) * tinySquaresAngles_.size());
  vbRectAngles_->setInstanceDataOffset(tinySquaresAngles_.data(), sizeToSend, offset);
  vbRectAngles_->unBind();
}

void TestRectangles::setVBScales() {
  GLintptr offset = 0;
  auto sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * smallRectScales_.size());
  vbRectScales_->setInstanceData(smallRectScales_.data(), sizeToSend, sizeof(glm::vec2) * kMaxRect);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * mediumRectScales_.size());
  vbRectScales_->setInstanceDataOffset(mediumRectScales_.data(), sizeToSend, offset);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * mainSquaresScales_.size());
  vbRectScales_->setInstanceDataOffset(mainSquaresScales_.data(), sizeToSend, offset);

  offset += sizeToSend;
  sizeToSend = static_cast<GLsizeiptr>(sizeof(glm::vec2) * tinySquaresScales_.size());
  vbRectScales_->setInstanceDataOffset(tinySquaresScales_.data(), sizeToSend, offset);
  vbRectScales_->unBind();
}

void TestRectangles::mainSquareCollisionRect() {
  const auto mainSquareTopLeft = glm::vec2(mainSquaresPositions_.at(0).x - TestRectangles::kRectangleSize,
                                           mainSquaresPositions_.at(0).y + TestRectangles::kRectangleSize);
  const auto mainSquareBottomRight = glm::vec2(mainSquaresPositions_.at(0).x + TestRectangles::kRectangleSize,
                                               mainSquaresPositions_.at(0).y - TestRectangles::kRectangleSize);

  for (unsigned int i = 0; i < smallRectPositions_.size(); ++i) {
    const auto& smallRectPos = smallRectPositions_.at(i);
    const auto smallRectTopLeft =
        glm::vec2(smallRectPos.x - TestRectangles::kRectangleSize, smallRectPos.y + TestRectangles::kRectangleSize);
    const auto smallRectBottomRight =
        glm::vec2(smallRectPos.x + TestRectangles::kRectangleSize, smallRectPos.y - TestRectangles::kRectangleSize);

    const bool collision =
        rectCollision(mainSquareTopLeft, mainSquareBottomRight, smallRectTopLeft, smallRectBottomRight);

    if (collision) {
      // Erase small rect
      this->eraseReact(true, i);
      // Grow main square
      mainSquaresScales_.at(0) += glm::vec2(0.2F, 0.2F);
      break;
    }
  }
  for (unsigned int i = 0; i < mediumRectPositions_.size(); ++i) {
    const auto& mediumRectPos = mediumRectPositions_.at(i);
    // Make a top left & bottom right to use `rectCollision`
    const auto mediumRectTopLeft =
        glm::vec2(mediumRectPos.x - TestRectangles::kRectangleSize, mediumRectPos.y + TestRectangles::kRectangleSize);
    const auto mediumRectBottomRight =
        glm::vec2(mediumRectPos.x + TestRectangles::kRectangleSize, mediumRectPos.y - TestRectangles::kRectangleSize);

    const bool collision =
        rectCollision(mainSquareTopLeft, mainSquareBottomRight, mediumRectTopLeft, mediumRectBottomRight);

    if (collision) {
      // Erase medium rect
      this->eraseReact(false, i);
      // Grow main square
      mainSquaresScales_.at(0) += glm::vec2(0.2F, 0.2F);
      break;
    }
  }
  this->setVBPositions();
  this->setVBAngles();
  this->setVBScales();
}

void TestRectangles::moveRectsToMainSquare() {
  for (auto& smallRectPos : smallRectPositions_) {
    glm::vec2 direction =
        glm::vec2(smallRectPos.x - mainSquaresPositions_.at(0).x, smallRectPos.y - mainSquaresPositions_.at(0).y);

    if (glm::abs(direction.x) > 0 || glm::abs(direction.y) > 0) {
      smallRectPos -= glm::normalize(direction) * TestRectangles::kRectMoveSpeed * deltaTime_;
    }
  }

  for (auto& mediumRectPos : mediumRectPositions_) {
    glm::vec2 direction =
        glm::vec2(mediumRectPos.x - mainSquaresPositions_.at(0).x, mediumRectPos.y - mainSquaresPositions_.at(0).y);

    if (glm::abs(direction.x) > 0 || glm::abs(direction.y) > 0) {
      mediumRectPos -= glm::normalize(direction) * TestRectangles::kRectMoveSpeed * deltaTime_;
    }
  }
  // Check for collisions?
  setVBPositions();
  // Risky not to update ALL buffers!
}

void TestRectangles::spawnTinySquares() {
  // Experimental
  tinySquaresPositions_.emplace_back(mainSquaresPositions_.at(0) + glm::vec2(-1.0F * 0.1F, 0.0F));
  tinySquaresPositions_.emplace_back(mainSquaresPositions_.at(0) + glm::vec2(-1.0F, 1.0F) * 0.1F);
  tinySquaresPositions_.emplace_back(mainSquaresPositions_.at(0) + glm::vec2(0.0F, 1.0F * 0.1F));
  tinySquaresPositions_.emplace_back(mainSquaresPositions_.at(0) + glm::vec2(1.0F, 1.0F) * 0.1F);

  for (unsigned int i = 0; i < 4; ++i) {
    tinySquaresAngles_.emplace_back(glm::radians(45.0F));
    tinySquaresScales_.emplace_back(0.5F, 0.5F);
  }

  this->setVBPositions();
  this->setVBAngles();
  this->setVBScales();
}

void TestRectangles::moveTinySquares() {
  if (tinySquaresPositions_.empty()) {
    return;
  }
  // Experimental
  std::array<glm::vec2, 4> directions = {glm::vec2(-1.0F, 1.0F), glm::vec2(1.0F, 1.0F), glm::vec2(-1.0F, -1.0F),
                                         glm::vec2(1.0F, -1.0F)};

  for (unsigned int i = 0; i < tinySquaresPositions_.size(); ++i) {
    const auto direction = directions.at(i % 4);
    auto& tinySquarePos = tinySquaresPositions_.at(i);
    tinySquarePos += direction * TestRectangles::kTinySquaresMoveSpeed * deltaTime_;

    if (glm::abs(tinySquarePos.x) > 0.5F || glm::abs(tinySquarePos.y) > 0.5F) {
      // TODO(cbr): make a function
      // TODO(cbr): figure out how to delete stuff without break (same for collisions)
      tinySquaresPositions_.erase(tinySquaresPositions_.begin() + i);
      tinySquaresAngles_.erase(tinySquaresAngles_.begin() + i);
      tinySquaresScales_.erase(tinySquaresScales_.begin() + i);
      break;
    }
  }
  this->setVBPositions();
  this->setVBAngles();
  this->setVBScales();
}

}  // namespace test