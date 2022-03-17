#ifndef TEST_DYNAMIC_RENDER_H_
#define TEST_DYNAMIC_RENDER_H_

#include <AspectRatio.h>
#include <IndexBuffer.h>
#include <KeyboardListener.h>
#include <Renderer.h>
#include <ShaderManager.h>
#include <Test.h>
#include <Texture.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <glmHeaders.h>

#include <memory>
#include <random>

#include "TileVertex.h"

namespace test {

class TestDynamicRender : public Test, public KeyboardListener {
 public:
  static constexpr unsigned int kDefaultGridRowColumnCount = 9;
  static constexpr unsigned int kDefaultGridSize = kDefaultGridRowColumnCount * kDefaultGridRowColumnCount;
  static constexpr unsigned int kDefaultGridVerticesCount = kDefaultGridSize * TileVertex::kTileVerticesCount;
  static constexpr unsigned int kDefaultGridVerticesFloatCount =
      kDefaultGridVerticesCount * TileVertex::kTileFloatValuesCount;
  static constexpr unsigned int kDefaultGridIndicesCount = kDefaultGridSize * TileVertex::kTileIndicesCount;

  static constexpr glm::vec3 perspectiveLookAtTarget = {0.0F, 0.0F, -1.0F};
  static constexpr glm::vec3 perspectiveLookAtUp = {0.0F, 1.0F, 0.0F};
  // Move camera: up on Z axis & back on the Y axis (to look from above and a bit behind)
  static constexpr glm::vec3 perspectiveLookAtPositionOffset = {0.0F, -0.5F, 5.0F};
  static constexpr float defaultCameraSpeed = 5.0F;

  static constexpr unsigned int kDefaultMaxDynamicTriangles = 16;
  static constexpr unsigned int kDefaultMaxDynamicTriangleIndices = kDefaultMaxDynamicTriangles * 3;
  // TODO(cbr): remove
  static constexpr unsigned int maxDynamicTriangles = 16;
  static constexpr unsigned int maxDynamicTriangleVertexValues =
      maxDynamicTriangles * 6;  // A triangle is made of 6 float values
  static constexpr unsigned int maxDynamicTriangleIndiceValues =
      maxDynamicTriangles * 3;  // A triangle indice is made of 3 unsigned int values

 public:
  explicit TestDynamicRender(const TestContext& ctx);
  ~TestDynamicRender() override;

  TestDynamicRender(const TestDynamicRender& other) = delete;
  TestDynamicRender(TestDynamicRender&& other) = delete;
  auto operator=(const TestDynamicRender& other) -> TestDynamicRender& = delete;
  auto operator=(TestDynamicRender&& other) -> TestDynamicRender& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

  void onMoveForward() override;
  void onMoveBackward() override;
  void onMoveLeft() override;
  void onMoveRight() override;
  void onZoomIn() override;
  void onZoomOut() override;

 private:
  std::array<float, 4> backgroundColor_ = {0.4F, 0.7F, 0.6F, 1.0F};
  float zoom_{7.0F};
  float fov_{90.0F};
  bool usePerspective_{false};
  AspectRatio& aspectRatio_;
  float reversedAspectRatio_{aspectRatio_.reversed()};
  float frameDeltaTime_{0.0F};

  float cameraPosX_{0.0F};
  float cameraPosY_{0.0F};

  std::array<TileVertex, kDefaultGridVerticesCount> gridVertices_{};
  int currentCameraTileIdx_ = -1;

  Renderer renderer_;
  std::unique_ptr<VertexArray> va1_;
  std::unique_ptr<VertexBuffer> vb1_;
  std::unique_ptr<IndexBuffer> ib1_;
  std::unique_ptr<ShaderManager> shader1_;
  std::unique_ptr<Texture> textureWall_;
  std::unique_ptr<Texture> textureGrass_;

  std::unique_ptr<VertexArray> va2_;
  std::unique_ptr<VertexBuffer> vb2_;
  std::unique_ptr<IndexBuffer> ib2_;
  std::unique_ptr<ShaderManager> shader2_;

  std::unique_ptr<VertexArray> va3_;
  std::unique_ptr<VertexBuffer> vb3_;
  std::unique_ptr<IndexBuffer> ib3_;
  std::unique_ptr<ShaderManager> shader3_;

  void setViewProjection(bool usePerspective, ShaderManager& shader);
  void setModel(ShaderManager& shader);
  void updateModelViewProjection();

  std::default_random_engine gen{std::random_device{}()};
  std::array<unsigned int, kDefaultMaxDynamicTriangleIndices> dynamicIndices_;
  std::vector<float> dynamicTriangleVertices_{};
  void addDynamicTriangle();
};

}  // namespace test

#endif