#ifndef TEST_CONTEXT_H_
#define TEST_CONTEXT_H_

#include "ThreadPoolManager.h"
#include "WindowManager.h"

namespace test {

struct TestContext {
  std::shared_ptr<WindowManager> windowManager;
  std::shared_ptr<ThreadPoolManager> threadPoolManager;
};

}  // namespace test
#endif