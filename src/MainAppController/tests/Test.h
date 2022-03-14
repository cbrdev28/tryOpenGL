#ifndef TEST_H_
#define TEST_H_

#include <TestContext.h>

namespace test {

class Test {
 public:
  explicit Test(const TestContext& testContext) : testContext_(testContext){};
  virtual ~Test() = default;

  Test(const Test& other) = delete;
  Test(Test&& other) = delete;
  auto operator=(const Test& other) -> Test& = delete;
  auto operator=(Test&& other) -> Test& = delete;

  virtual void onUpdate(float /* deltaTime */) {}
  virtual void onRender() {}
  virtual void onImGuiRender() {}

  inline auto getTestContext() -> const TestContext& { return testContext_; }

 private:
  const TestContext& testContext_;
};

}  // namespace test
#endif