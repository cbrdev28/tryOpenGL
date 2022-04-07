#ifndef TEST_GAME_MANAGER_H_
#define TEST_GAME_MANAGER_H_

#include "Test.h"

namespace test {

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
};

}  // namespace test

#endif