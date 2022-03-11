#ifndef TEST_TEXTURE_H_
#define TEST_TEXTURE_H_

#include <IndexBuffer.h>
#include <ShaderManager.h>
#include <Test.h>
#include <VertexArray.h>
#include <VertexBuffer.h>

#include <memory>

#include "Renderer.h"

namespace test {

class TestTexture : public Test {
 public:
  explicit TestTexture(const TestContext& ctx);
  ~TestTexture() override;

  TestTexture(const TestTexture& other) = delete;
  TestTexture(TestTexture&& other) = delete;
  auto operator=(const TestTexture& other) -> TestTexture& = delete;
  auto operator=(TestTexture&& other) -> TestTexture& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  std::vector<float> backgroundColor_ = {1.0F, 1.0F, 1.0F, 1.0F};

  std::unique_ptr<VertexArray> va_;
  std::unique_ptr<VertexBuffer> vb_;
  std::unique_ptr<IndexBuffer> ib_;
  std::unique_ptr<ShaderManager> shader_;

  Renderer renderer_;
};

}  // namespace test

#endif