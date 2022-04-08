#ifndef TEST_WORLD_MANAGER_H_
#define TEST_WORLD_MANAGER_H_

#include <Test.h>
#include <WorldManager.h>

namespace test {
class TestWorldManager : public Test {
 public:
  explicit TestWorldManager(const TestContext& ctx);
  ~TestWorldManager() override;

  TestWorldManager(const TestWorldManager& other) = delete;
  TestWorldManager(TestWorldManager&& other) = delete;
  auto operator=(const TestWorldManager& other) -> TestWorldManager& = delete;
  auto operator=(TestWorldManager&& other) -> TestWorldManager& = delete;

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  WorldManager worldManager_;
};
}  // namespace test
#endif