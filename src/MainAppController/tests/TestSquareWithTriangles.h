#ifndef TEST_SQUARE_WITH_TRIANGLES_H_
#define TEST_SQUARE_WITH_TRIANGLES_H_

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

class TestSquareWithTriangles : public Test {
 public:
  explicit TestSquareWithTriangles(const TestContext& ctx);
  ~TestSquareWithTriangles() override;

  TestSquareWithTriangles(const TestSquareWithTriangles& other) = delete;
  TestSquareWithTriangles(TestSquareWithTriangles&& other) = delete;
  auto operator=(const TestSquareWithTriangles& other) -> TestSquareWithTriangles& = delete;
  auto operator=(TestSquareWithTriangles&& other) -> TestSquareWithTriangles& = delete;

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