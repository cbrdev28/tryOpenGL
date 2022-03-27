#ifndef TEST_INSTANCE_TRIANGLE_H_
#define TEST_INSTANCE_TRIANGLE_H_

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

class TestInstanceTriangle : public Test {
 public:
  explicit TestInstanceTriangle(const TestContext& ctx);
  ~TestInstanceTriangle() override;

  TestInstanceTriangle(const TestInstanceTriangle& other) = delete;
  TestInstanceTriangle(TestInstanceTriangle&& other) = delete;
  auto operator=(const TestInstanceTriangle& other) -> TestInstanceTriangle& = delete;
  auto operator=(TestInstanceTriangle&& other) -> TestInstanceTriangle& = delete;

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