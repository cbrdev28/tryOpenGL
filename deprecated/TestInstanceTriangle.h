#ifndef TEST_INSTANCE_TRIANGLE_H_
#define TEST_INSTANCE_TRIANGLE_H_

#include <array>
#include <memory>

#include "IndexBuffer.h"
#include "InstancedTriangle.h"
#include "Renderer.h"
#include "Shader.h"
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
  std::unique_ptr<VertexBuffer> vbModelPositions2_;
  std::unique_ptr<VertexBuffer> vbModelZRotationAngle3_;
  std::unique_ptr<Shader> shader_;

  std::unique_ptr<InstancedTriangle> instancedTriangle_ = std::make_unique<InstancedTriangle>();
  void addTriangleInstance(int count = 1);
  int newInstancesCount_{1};

  bool useThreads_{false};
  std::string debugUpdateStatus_{"Idle"};
  void onThreadedUpdate(float dt);
};

}  // namespace test

#endif