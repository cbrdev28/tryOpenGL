#ifndef TEST_RENDER_TILES_H_
#define TEST_RENDER_TILES_H_

#include <AspectRatio.h>
#include <IndexBuffer.h>
#include <KeyboardListener.h>
#include <Renderer.h>
#include <ShaderManager.h>
#include <Test.h>
#include <Texture.h>
#include <TileVertex.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <glmHeaders.h>

#include <memory>

namespace test {

class TestRenderTiles : public Test, public KeyboardListener {
 public:
  static constexpr unsigned int maxGridSize = 64;
  static constexpr float tileSize = 1.0F;
  static constexpr float tileSpacing = 0.001F;
  static constexpr unsigned int verticesPerTile = 4;
  static constexpr unsigned int indicesPerTile = 6;
  static constexpr glm::vec3 perspectiveLookAtTarget = {0.0F, 0.0F, -1.0F};
  static constexpr glm::vec3 perspectiveLookAtUp = {0.0F, 1.0F, 0.0F};
  // Move camera: up on Z axis & back on the Y axis (to look from above and a bit behind)
  static constexpr glm::vec3 perspectiveLookAtPositionOffset = {0.0F, -0.5F, 5.0F};
  static constexpr unsigned int gridRowColumnCount = 9;
  static constexpr unsigned int gridSize = gridRowColumnCount * gridRowColumnCount;
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
  AspectRatio& aspectRatio_;
  float reversedAspectRatio_{aspectRatio_.reversed()};
  bool usePerspective_{false};
  float frameDeltaTime_{0.0F};

  float cameraPosX_{0.0F};
  float cameraPosY_{0.0F};
  std::vector<TileVertex> tileVertices_{};
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
  auto makeTilesVertices(unsigned int size) -> std::vector<TileVertex>;
  auto makeTilesIndices(unsigned int tileVerticesCount) -> std::vector<unsigned int>;
  auto findTileBaseIdxForPos(float posX, float posY, const std::vector<TileVertex>& vertices) -> int;
};

}  // namespace test

#endif