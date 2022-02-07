#include <iostream>

#include "src/simple-tests.h"

int main(int argc, char **argv) {
  if (SimpleTests::ArgHasTest(argc, argv) == true) {
    return SimpleTests::ParseArgAndRun(argc, argv);
  }

  std::cout << "Hello world" << std::endl;
  return 0;
}