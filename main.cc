#include <iostream>

#include "src/simple-tests.h"

int main(int argc, char **argv) {
  SimpleTests::SelfTest();

  std::cout << "Hello world "
            << "argc: " << argc << " argv: " << argv[0] << std::endl;
  return 0;
}