#ifndef TEST_RENDER_TILES_H_
#define TEST_RENDER_TILES_H_

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

#include "TileVertex.h"

namespace test {

class TestRenderTiles : public Test, public KeyboardListener {
 public:
  static constexpr unsigned int kDefaultGridRowColumnCount = 9;
  static constexpr unsigned int kDefaultGridSize = kDefaultGridRowColumnCount * kDefaultGridRowColumnCount;
  static constexpr unsigned int kDefaultGridVerticesCount = kDefaultGridSize * TileVertex::kTileVerticesCount;
  static constexpr unsigned int kDefaultGridVerticesFloatCount =
      kDefaultGridVerticesCount * TileVertex::kTileFloatValuesCount;
  static constexpr unsigned int kDefaultGridIndicesCount = kDefaultGridSize * TileVertex::kTileIndicesCount;

  static constexpr glm::vec3 perspectiveLookAtTarget = {0.0F, 0.0F, -1.0F};
  static constexpr glm::vec3 perspectiveLookAtUp = {0.0F, 1.0F, 0.0F};
  static constexpr glm::vec3 perspectiveLookAtPositionOffset = {0.0F, -0.5F, 5.0F};
  static constexpr float defaultCameraSpeed = 5.0F;

 public:
  explicit TestRenderTiles(const TestContext& ctx);
  ~TestRenderTiles() override;

  TestRenderTiles(const TestRenderTiles& other) = delete;
  TestRenderTiles(TestRenderTiles&& other) = delete;
  auto operator=(const TestRenderTiles& other) -> TestRenderTiles& = delete;
  auto operator=(TestRenderTiles&& other) -> TestRenderTiles& = delete;

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
  std::vector<float> backgroundColor_ = {0.5F, 0.4F, 0.3F, 1.0F};
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

  void setViewProjection(bool usePerspective, ShaderManager& shader);
  void setModel(ShaderManager& shader);
  void updateModelViewProjection();
};

}  // namespace test

#endif