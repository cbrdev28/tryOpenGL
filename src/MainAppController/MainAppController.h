#ifndef MAIN_APP_CONTROLLER_H_
#define MAIN_APP_CONTROLLER_H_

#include <memory>

#include "ImGuiManager.h"
#include "Renderer.h"
#include "TestContext.h"
#include "TestMenu.h"
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
  ImGuiManager imGuiManager_{*windowManager_};
  Renderer renderer_;
  test::TestContext testCtx_{windowManager_};
  test::TestMenu testMenu_{testCtx_};

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
