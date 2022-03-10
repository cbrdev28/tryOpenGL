#ifndef TEST_H_
#define TEST_H_

namespace test {

class Test {
 public:
  Test() = default;
  virtual ~Test() = default;

  Test(const Test& other) = delete;
  Test(Test&& other) = delete;
  auto operator=(const Test& other) -> Test& = delete;
  auto operator=(Test&& other) -> Test& = delete;

  virtual void onUpdate(float deltaTime) {}
  virtual void onRender() {}
  virtual void onImGuiRender() {}
};

}  // namespace test
#endif