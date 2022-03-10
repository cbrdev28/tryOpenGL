/**
 * This file is the main app controller: the first entry in our app.
 */
#include "MainAppController.h"

#include <TestMenu.h>
#include <TestWorldManager.h>
#include <fmt/core.h>

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
    imGuiManager_.init();
  } catch (int error) {
    return -1;
  }
  return 0;
}

auto MainAppController::renderLoop() -> MainAppController& {
  fmt::print("renderLoop()\n");

  test::TestMenu testMenu;
  testMenu.registerTest<test::TestWorldManager>("Test World");

  GLFWwindow* window = windowManager_.getWindow();
  while (glfwWindowShouldClose(window) == 0) {
    inputManager_.processKeyboardInput();
    renderer_.clear();
    imGuiManager_.renderFrame();

    testMenu.onUpdate(0.0F);
    testMenu.onRender();
    testMenu.onImGuiRender();

    // imGuiManager_.renderExample();
    imGuiManager_.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return *this;
}