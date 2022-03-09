/**
 * This file is the main app controller: the first entry in our app.
 */
#include "MainAppController.h"

#include <fmt/core.h>

// Constructor
MainAppController::MainAppController() { fmt::print("MainAppController(...)\n"); };

auto MainAppController::run() -> int {
  fmt::print("run()\n");
  int initialized = this->init();
  if (initialized != 0) {
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
    inputManager_.init();
    worldManager_.init();
  } catch (int error) {
    return -1;
  }
  return 0;
}

auto MainAppController::renderLoop() -> MainAppController& {
  fmt::print("renderLoop()\n");

  GLFWwindow* window = windowManager_.getWindow();
  while (glfwWindowShouldClose(window) == 0) {
    inputManager_.processKeyboardInput();

    worldManager_.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return *this;
}