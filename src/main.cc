#include <MainAppController.h>

#include <memory>

#include "MainAppController.h"

auto main(/* int argc, char** argv */) -> int {
  std::unique_ptr<MainAppController> mainApp = std::make_unique<MainAppController>();
  auto mainAppResult = mainApp->run();
  return mainAppResult;
}
