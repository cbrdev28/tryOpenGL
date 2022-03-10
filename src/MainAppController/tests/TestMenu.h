#include <fmt/core.h>

#include <functional>
#include <gsl/gsl>
#include <string>
#include <utility>
#include <vector>

#include "Test.h"

namespace test {

class TestMenu : public Test {
 public:
  template <typename S>
  void registerTest(const std::string& name) {
    tests_.push_back(std::make_pair(name, &TestMenu::newTest<S>));
  }

  template <typename T>
  static auto newTest() -> gsl::owner<Test*> {
    return new T();
  }

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  Test* currentTest_{nullptr};
  std::vector<std::pair<std::string, std::function<Test*()>>> tests_;
};

}  // namespace test
