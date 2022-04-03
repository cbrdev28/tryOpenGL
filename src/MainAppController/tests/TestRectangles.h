#ifndef TEST_RECTANGLES_H_
#define TEST_RECTANGLES_H_

#include <array>
#include <memory>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "WindowListener.h"
#include "glmHeaders.h"

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

  void onKeyADown() override;
  void onKeySDown() override;
  void onKeyDDown() override;
  void onKeyWDown() override;

 private:
  std::array<float, 4> backgroundColor_ = {0.8F, 0.8F, 0.8F, 1.0F};
  float deltaTime_{0.0F};

  Renderer renderer_;
  std::unique_ptr<VertexArray> vaRectangles_;
  std::unique_ptr<VertexBuffer> vbRectanglesVertices_;
  std::unique_ptr<VertexBuffer> vbRectanglesPositions_;
  std::unique_ptr<VertexBuffer> vbRectanglesZAngles_;
  std::unique_ptr<Shader> shaderRectangles_;

  bool useThreads_{false};
  void onThreadedUpdate(float dt);
};

}  // namespace test

#endif