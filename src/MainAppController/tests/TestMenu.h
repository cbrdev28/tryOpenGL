#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "Test.h"

namespace test {

class TestMenu : public Test {
 public:
  explicit TestMenu(Test*& currentTestPtr);

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;

 private:
  Test*& currentTest_;
  std::vector<std::pair<std::string, std::function<Test*()>>> tests_;
};

}  // namespace test
