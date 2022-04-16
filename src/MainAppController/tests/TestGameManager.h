#ifndef TEST_GAME_MANAGER_H_
#define TEST_GAME_MANAGER_H_

#include "SceneSelectCharacter.h"
#include "SceneTraining.h"
#include "Test.h"

namespace test {

struct Scene {
  std::string name;
  std::function<std::unique_ptr<Test>()> create;

  // If true: the scene will only be enabled if a current character is set
  bool dependsOnCurrentCharacter;
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
  static constexpr unsigned int kMaxScene = 2;
  std::array<Scene, kMaxScene> scenes_{
      {{"Select Character", [&]() { return std::make_unique<SceneSelectCharacter>(this->getTestContext()); }, false},
       {"Training", [&]() { return std::make_unique<SceneTraining>(this->getTestContext()); }, true}}};
};

}  // namespace test

#endif