#include "cbrpch.h"

#include "MainAppController.h"

auto main(/* int argc, char** argv */) -> int {
  std::unique_ptr<MainAppController> mainApp = std::make_unique<MainAppController>();
  int mainAppRes = mainApp->run();
  return mainAppRes;
}
