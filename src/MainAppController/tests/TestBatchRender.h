#ifndef TEST_BATCH_RENDER_H_
#define TEST_BATCH_RENDER_H_

#include <IndexBuffer.h>
#include <Renderer.h>
#include <ShaderManager.h>
#include <Test.h>
#include <Texture.h>
#include <VertexArray.h>
#include <VertexBuffer.h>

#include <memory>

namespace test {

class TestBatchRender : public Test {
 public:
  explicit TestBatchRender(const TestContext& ctx);
  ~TestBatchRender() override;

  TestBatchRender(const TestBatchRender& other) = delete;
  TestBatchRender(TestBatchRender&& other) = delete;
  auto operator=(const TestBatchRender& other) -> TestBatchRender& = delete;
  auto operator=(TestBatchRender&& other) -> TestBatchRender& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  std::vector<float> backgroundColor_ = {1.0F, 0.5F, 0.1F, 1.0F};
  Renderer renderer_;
  std::unique_ptr<VertexArray> va_;
  std::unique_ptr<VertexBuffer> vb_;
  std::unique_ptr<IndexBuffer> ib_;
  std::unique_ptr<ShaderManager> shader_;
  std::unique_ptr<Texture> texture_;
};

}  // namespace test

#endif