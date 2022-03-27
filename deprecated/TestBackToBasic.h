#ifndef TEST_BACK_TO_BASIC_H_
#define TEST_BACK_TO_BASIC_H_

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

class TestBackToBasic : public Test {
 public:
  explicit TestBackToBasic(const TestContext& ctx);
  ~TestBackToBasic() override;

  TestBackToBasic(const TestBackToBasic& other) = delete;
  TestBackToBasic(TestBackToBasic&& other) = delete;
  auto operator=(const TestBackToBasic& other) -> TestBackToBasic& = delete;
  auto operator=(TestBackToBasic&& other) -> TestBackToBasic& = delete;

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