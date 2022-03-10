#include <functional>
#include <gsl/gsl>
#include <string>
#include <utility>
#include <vector>

#include "Test.h"

namespace test {

class TestMenu : public Test {
 public:
  explicit TestMenu(const TestContext& ctx) : Test(ctx) {}

  template <typename S>
  void registerTest(const std::string& name) {
    tests_.push_back(std::make_pair(name, &TestMenu::newTest<S>));
  }

  template <typename T>
  static auto newTest(const TestContext& ctx) -> gsl::owner<Test*> {
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
