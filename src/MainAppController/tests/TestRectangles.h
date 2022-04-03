#ifndef TEST_RECTANGLES_H_
#define TEST_RECTANGLES_H_

#include <array>
#include <memory>
#include <random>
#include <vector>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "WindowListener.h"
#include "glmHeaders.h"
#include "openGLHeaders.h"

namespace test {

class TestRectangles : public Test, public WindowListener {
 public:
  explicit TestRectangles(const TestContext& ctx);
  ~TestRectangles() override;

  TestRectangles(const TestRectangles& other) = delete;
  TestRectangles(TestRectangles&& other) = delete;
  auto operator=(const TestRectangles& other) -> TestRectangles& = delete;
  auto operator=(TestRectangles&& other) -> TestRectangles& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  std::array<float, 4> backgroundColor_ = {0.8F, 0.8F, 0.8F, 1.0F};
  float deltaTime_{0.0F};

  Renderer renderer_;
  std::unique_ptr<VertexArray> vaRect_ = std::make_unique<VertexArray>();
  std::unique_ptr<VertexBuffer> vbRectVertices_;
  std::unique_ptr<VertexBuffer> vbRectPositions_;
  std::unique_ptr<VertexBuffer> vbRectAngles_;
  std::unique_ptr<Shader> shaderRect_ = std::make_unique<Shader>("test_rectangles.shader");

  bool useThreads_{false};
  void onThreadedUpdate(float dt);

  void onKeyADown() override;
  void onKeySDown() override;
  void onKeyDDown() override;
  void onKeyWDown() override;

  static constexpr GLsizei kMaxSmallRect = 1000;
  static constexpr GLsizei kMaxMediumRect = 100;
  static constexpr GLsizei kMaxRect = kMaxSmallRect + kMaxMediumRect;
  static constexpr GLfloat kRotationSpeed = 100.0F;
  static constexpr GLfloat kRotationAngle = glm::radians(1.0F);

  static constexpr GLfloat kRectangleSize = 0.05F;
  std::array<glm::vec2, 6> rectVertices = {
      glm::vec2(-kRectangleSize / 2.0F, -kRectangleSize / 2.0F),
      glm::vec2(kRectangleSize / 2.0F, -kRectangleSize / 2.0F),
      glm::vec2(kRectangleSize / 2.0F, kRectangleSize / 2.0F),

      glm::vec2(kRectangleSize / 2.0F, kRectangleSize / 2.0F),
      glm::vec2(-kRectangleSize / 2.0F, kRectangleSize / 2.0F),
      glm::vec2(-kRectangleSize / 2.0F, -kRectangleSize / 2.0F),
  };

  std::vector<glm::vec2> smallRectPositions_;
  std::vector<GLfloat> smallRectAngles_;

  std::vector<glm::vec2> mediumRectPositions_;
  std::vector<GLfloat> mediumRectAngles_;

  // Helper
  inline auto currentRectCount() -> GLsizei {
    return static_cast<GLsizei>(smallRectPositions_.size() + mediumRectPositions_.size());
  }

  // Helper to return random number within range of: 0.0 - 1.0
  std::default_random_engine gen{std::random_device{}()};
  auto genRandom() -> GLfloat {
    std::uniform_real_distribution<GLfloat> dist(0.0F, 1.0F);
    return dist(gen);
  }
  // Spawn rectangle with random positions & away from given target
  // For now, we only specify whether the rectangle is small or not
  void spawnReact(const bool& small, const glm::vec2& target = {0.0F, 0.0F});

  void setVBPositions();
  void setVBAngles();
};

}  // namespace test

#endif