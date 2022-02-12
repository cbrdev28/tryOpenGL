#include <MainAppController.h>

#include <iostream>

#include "src/simple-tests.h"

int main(int argc, char** argv) {
  if (SimpleTests::ArgHasTest(argc, argv) == true) {
    return SimpleTests::ParseArgAndRun(argc, argv);
  }

  const MainAppController mainAppController = MainAppController(argc, argv);
  return 0;
}
