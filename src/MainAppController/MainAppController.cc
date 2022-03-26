/**
 * This file is the main app controller: the first entry in our app.
 */
#include "MainAppController.h"

#include <fmt/core.h>

auto MainAppController::run() -> int {
  fmt::print("run()\n");
  if (this->init() != 0) {
    return -1;
  }
  try {
    this->renderLoop();
  } catch (int error) {
    return -1;
  }
  return 0;
}

auto MainAppController::init() -> int {
  fmt::print("init()\n");
  try {
    windowManager_.init();
    imGuiManager_.init();
  } catch (int error) {
    return -1;
  }
  return 0;
}

auto MainAppController::renderLoop() -> void {
  fmt::print("renderLoop()\n");

  GLFWwindow* window = windowManager_.getWindow();
  while (glfwWindowShouldClose(window) == 0) {
    windowManager_.updateWindowStats();
    renderer_.clear();

    imGuiManager_.renderFrame();

    testMenu_.onUpdate(windowManager_.getWindowStats().frameDeltaTime.count());
    testMenu_.onRender();
    testMenu_.onImGuiRender();

    // imGuiManager_.renderExample();
    imGuiManager_.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}