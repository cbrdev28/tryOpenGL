#include <MainAppController.h>

auto main(/* int argc, char** argv */) -> int {
  MainAppController mainAppController;
  const int mainAppResult = mainAppController.run();
  return mainAppResult;
}
