#ifndef TEST_CONTEXT_H_
#define TEST_CONTEXT_H_

#include "GameManager.h"
#include "ThreadPoolManager.h"
#include "WindowManager.h"

namespace test {

struct TestContext {
  std::shared_ptr<WindowManager> windowManager;
  std::shared_ptr<ThreadPoolManager> threadPoolManager;
  std::shared_ptr<GameManager> gameManager;
};

}  // namespace test
#endif