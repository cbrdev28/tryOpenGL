#ifndef TEST_CONTEXT_H_
#define TEST_CONTEXT_H_

#include <InputManager.h>
#include <WindowManager.h>

namespace test {

struct TestContext {
  WindowManager& windowManager;
  InputManager& inputManager;
};

}  // namespace test
#endif