#include "MainAppController.h"

auto main(/* int argc, char** argv */) -> int {
  MainAppController *mainApp = new MainAppController();
  int mainAppRes = mainApp->run();

  delete mainApp;
  return mainAppRes;
}
