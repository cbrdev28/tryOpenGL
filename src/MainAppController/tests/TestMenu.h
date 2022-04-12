#ifndef TEST_MENU_H_
#define TEST_MENU_H_

#include "Test.h"

namespace test {

class TestMenu : public Test {
 public:
  explicit TestMenu(const TestContext& ctx) : Test(ctx) {}
  ~TestMenu() override;

  TestMenu(const TestMenu& other) = delete;
  TestMenu(TestMenu&& other) = delete;
  auto operator=(const TestMenu& other) -> TestMenu& = delete;
  auto operator=(TestMenu&& other) -> TestMenu& = delete;

  template <typename S>
  inline void registerTest(const std::string& name) {
    tests_.emplace_back(std::make_pair(name, &TestMenu::newTest<S>));
  }

  template <typename T>
  inline static auto newTest(const TestContext& ctx) -> gsl::owner<Test*> {
    return new T(ctx);
  }

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  Test* currentTest_{nullptr};
  std::vector<std::pair<std::string, std::function<Test*(const TestContext&)>>> tests_;
};

}  // namespace test

#endif