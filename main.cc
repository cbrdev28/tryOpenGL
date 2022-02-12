#include <MainAppController.h>

#include <iostream>

#include "src/simple-tests.h"

int main(int argc, char** argv) {
  if (SimpleTests::ArgHasTest(argc, argv) == true) {
    return SimpleTests::ParseArgAndRun(argc, argv);
  }

  MainAppController mainAppController = MainAppController(argc, argv);
  const int mainAppResult = mainAppController.run();
  return mainAppResult;
}
