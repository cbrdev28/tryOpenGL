/**
 * This file is the main app controller: the first entry in our app.
 */
#include "MainAppController.h"

#include <TestWorldManager.h>
#include <WorldManager.h>
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

  WorldManager world(windowManager_, inputManager_, renderer_);
  test::TestWorldManager testWorld(world);

  GLFWwindow* window = windowManager_.getWindow();
  while (glfwWindowShouldClose(window) == 0) {
    inputManager_.processKeyboardInput();
    renderer_.clear();
    imGuiManager_.renderFrame();

    testWorld.onUpdate(0.0F);
    testWorld.onRender();
    testWorld.onImGuiRender();

    // imGuiManager_.renderExample();
    imGuiManager_.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return *this;
}