/**
 * This file is the main app controller: the first entry in our app.
 */

#include "MainAppController.h"

#include <fmt/core.h>

// Constructor
MainAppController::MainAppController(const int argc, const char* const* const argv) : _argc(argc), _argv(argv) {
  fmt::print("MainAppController(...)\n");
};

int MainAppController::run() {
  fmt::print("run()\n");
  return 0;
}