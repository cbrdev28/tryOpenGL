/**
 * This file is the main app controller: the first entry in our app.
 */
#include "MainAppController.h"

#include <TestBatchRender.h>
#include <TestRenderTiles.h>
#include <TestTexture.h>
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

    testMenu_.registerTest<test::TestWorldManager>("Test World");
    testMenu_.registerTest<test::TestTexture>("Test Texture?");
    testMenu_.registerTest<test::TestBatchRender>("Test Batch Rendering");
    testMenu_.registerTest<test::TestRenderTiles>("Test Render Tiles");
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
  return *this;
}