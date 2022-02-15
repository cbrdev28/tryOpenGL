#include <MainAppController.h>

int main(int argc, char** argv) {
  MainAppController mainAppController = MainAppController(argc, argv);
  const int mainAppResult = mainAppController.run();
  return mainAppResult;
}
