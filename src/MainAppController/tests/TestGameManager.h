#ifndef TEST_GAME_MANAGER_H_
#define TEST_GAME_MANAGER_H_

#include "Test.h"

namespace test {

struct Scene {
  std::string name;
  std::function<std::unique_ptr<Test>()> create;
};

class TestGameManager : public Test {
 public:
  explicit TestGameManager(const TestContext& ctx) : Test(ctx) {}
  ~TestGameManager() override;

  TestGameManager(const TestGameManager& other) = delete;
  TestGameManager(TestGameManager&& other) = delete;
  auto operator=(const TestGameManager& other) -> TestGameManager& = delete;
  auto operator=(TestGameManager&& other) -> TestGameManager& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  float deltaTime_{0.0F};

  std::unique_ptr<Test> currentScene_{nullptr};
  static constexpr unsigned int kMaxScene = 1;
  std::array<Scene, kMaxScene> scenes_{
      {{"Test Scene 1", [&]() { return std::make_unique<TestGameManager>(this->getTestContext()); }}}};
};

}  // namespace test

#endif