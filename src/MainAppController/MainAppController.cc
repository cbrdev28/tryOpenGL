/**
 * This file is the main app controller: the first entry in our app.
 */
#include "MainAppController.h"

#include <fmt/core.h>

// Constructor
MainAppController::MainAppController(const int argc, const char* const* const argv)
    : _argc(argc), _argv(argv), _inputManager(_windowManager) {
  fmt::print("MainAppController(...)\n");
};

int MainAppController::run() {
  fmt::print("run()\n");
  int functionResult = this->init();
  return functionResult;
}

int MainAppController::init() {
  fmt::print("init()\n");
  try {
    this->_windowManager.init();
    this->_inputManager.init();
  } catch (int error) {
    return -1;
  }
  return 0;
}