#ifndef TEST_TEXTURE_H_
#define TEST_TEXTURE_H_

#include <Test.h>

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
};

}  // namespace test

#endif