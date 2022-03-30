#ifndef TEST_CONTEXT_H_
#define TEST_CONTEXT_H_

#include <memory>

#include "WindowManager.h"

namespace test {

struct TestContext {
  std::shared_ptr<WindowManager> windowManager;
};

}  // namespace test
#endif