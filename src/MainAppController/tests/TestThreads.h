#ifndef TEST_THREADS_H_
#define TEST_THREADS_H_

#include <array>
#include <memory>

#include "IndexBuffer.h"
#include "InstancedTriangle.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test {

class TestThreads : public Test {
 public:
  explicit TestThreads(const TestContext& ctx);
  ~TestThreads() override;

  TestThreads(const TestThreads& other) = delete;
  TestThreads(TestThreads&& other) = delete;
  auto operator=(const TestThreads& other) -> TestThreads& = delete;
  auto operator=(TestThreads&& other) -> TestThreads& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  std::array<float, 4> backgroundColor_ = {0.8F, 0.8F, 0.8F, 1.0F};
  float deltaTime_{0.0F};

  Renderer renderer_;
  std::unique_ptr<VertexArray> va_;
  std::unique_ptr<VertexBuffer> vbModelVertex0_;
  std::unique_ptr<VertexBuffer> vbModelTransformation2_;
  std::unique_ptr<ShaderManager> shader_;

  InstancedTriangle instancedTriangle_;
  void addTriangleInstance();
};

}  // namespace test

#endif