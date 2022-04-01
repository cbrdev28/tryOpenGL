#ifndef TEST_SQUARE_WITH_TRIANGLES_H_
#define TEST_SQUARE_WITH_TRIANGLES_H_

#include <array>
#include <memory>

#include "IndexBuffer.h"
#include "InstancedTriangle.h"
#include "Renderer.h"
#include "Shader.h"
#include "TargetSquare.h"
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "WindowListener.h"
#include "glmHeaders.h"

namespace test {

class TestSquareWithTriangles : public Test, public WindowListener {
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

  void onKeyADown() override;

 private:
  std::array<float, 4> backgroundColor_ = {0.8F, 0.8F, 0.8F, 1.0F};
  float deltaTime_{0.0F};

  Renderer renderer_;
  std::unique_ptr<VertexArray> vaTriangles_;
  std::unique_ptr<VertexBuffer> vbTrianglesVertices_;
  std::unique_ptr<VertexBuffer> vbTrianglesPositions_;
  std::unique_ptr<VertexBuffer> vbTrianglesZAngles_;
  std::unique_ptr<Shader> shaderTriangles_;
  void initTriangles();

  std::unique_ptr<InstancedTriangle> instancedTriangle_ = std::make_unique<InstancedTriangle>();
  void addTriangleInstance(int count = 1);
  int newInstancesCount_{1};

  bool useThreads_{false};
  std::string debugUpdateStatus_{"Idle"};
  void onThreadedUpdate(float dt);

  std::unique_ptr<VertexArray> vaSquare_;
  std::unique_ptr<VertexBuffer> vbSquareVertices_;
  std::unique_ptr<VertexBuffer> vbSquarePosition_;
  std::unique_ptr<VertexBuffer> vbSquareZAngle_;
  std::unique_ptr<Shader> shaderSquare_;
  void initSquare();

  std::unique_ptr<TargetSquare> targetSquare_ = std::make_unique<TargetSquare>();
};

}  // namespace test

#endif