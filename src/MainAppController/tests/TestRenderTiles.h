#ifndef TEST_RENDER_TILES_H_
#define TEST_RENDER_TILES_H_

#include <IndexBuffer.h>
#include <Renderer.h>
#include <ShaderManager.h>
#include <Test.h>
#include <Texture.h>
#include <VertexArray.h>
#include <VertexBuffer.h>

#include <memory>

namespace test {

class TestRenderTiles : public Test {
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

 private:
  std::vector<float> backgroundColor_ = {0.5F, 0.4F, 0.3F, 1.0F};
  float zoom_{10.0F};
  float deltaX_{0.0F};
  float deltaY_{0.0F};
  float fov_{45.0F};
  bool usePerspective_{false};

  Renderer renderer_;
  std::unique_ptr<VertexArray> va_;
  std::unique_ptr<VertexBuffer> vb_;
  std::unique_ptr<IndexBuffer> ib_;
  std::unique_ptr<ShaderManager> shader_;
  std::unique_ptr<Texture> textureWall_;
  std::unique_ptr<Texture> textureGrass_;

  void setViewProjection(bool usePerspective);
};

}  // namespace test

#endif