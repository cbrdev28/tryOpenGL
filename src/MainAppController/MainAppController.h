#ifndef MAIN_APP_CONTROLLER_H_
#define MAIN_APP_CONTROLLER_H_

#include "GameManager.h"
#include "ImGuiManager.h"
#include "Renderer.h"
#include "TestContext.h"
#include "TestMenu.h"
#include "ThreadPoolManager.h"
#include "WindowManager.h"

/**
 * This file is the main app controller: the first entry in our app.
 */
class MainAppController {
 public:
  /**
   * Run the main application
   * @return -1 if an error occurred, 0 otherwise
   */
  auto run() -> int;

 private:
  std::shared_ptr<WindowManager> windowManager_ = std::make_shared<WindowManager>();
  std::unique_ptr<ImGuiManager> imGuiManager_ = std::make_unique<ImGuiManager>(*windowManager_);
  std::shared_ptr<ThreadPoolManager> threadPoolManager_ = std::make_shared<ThreadPoolManager>();
  std::shared_ptr<GameManager> gameManager_ = std::make_shared<GameManager>();

  test::TestContext testCtx_{windowManager_, threadPoolManager_, gameManager_};
  std::unique_ptr<test::TestMenu> testMenu_ = std::make_unique<test::TestMenu>(testCtx_);

  Renderer renderer_;

  /**
   * Initialize the app & managers
   * @return -1 if an error occurred, 0 otherwise
   */
  auto init() -> int;

  /**
   * Start a basic render loop
   */
  void renderLoop();
};

#endif
