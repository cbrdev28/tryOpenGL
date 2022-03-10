#ifndef TEST_WORLD_MANAGER_H_
#define TEST_WORLD_MANAGER_H_

#include <Test.h>
#include <WorldManager.h>

namespace test {
class TestWorldManager : public Test {
 public:
  explicit TestWorldManager(const TestContext& ctx);

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  WorldManager worldManager_;
};
}  // namespace test
#endif