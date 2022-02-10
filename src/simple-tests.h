/**
 * This file is meant to do a simple parsing of the args
 * to check if we have:
 * - "--test": to print the self test "Hello tests"
 * - TODO: "--test first-window": to play with glfw, opengl, ...
 * - TODO: and more stuff that I will forget to document
 */

#include <glad/glad.h>
#include <string.h>

#include <iostream>

/**
 * A simple helper class to parse arguments and run some tests
 */
class SimpleTests {
 public:
  /**
   * Constants
   */
  static const int MIN_ARG_FOR_TEST = 2;
  static constexpr const char* TEST_ARG = "--test";
  static constexpr const char* FIRST_WINDOW_TEST_ARG = "first-window";

  /**
   * Check if arguments from main function include "--test"
   * @param argc the argc from main function
   * @param argv the argv from main function
   * @return true if "--test" was found in arg, false otherwise
   */
  static bool ArgHasTest(int argc, char** argv) {
    if (argc < MIN_ARG_FOR_TEST) {
      return false;
    }

    if (strcmp(argv[1], TEST_ARG) == 0) {
      return true;
    }
    return false;
  }

  /**
   * Parse argv & argc to determine which test to run
   * @param argc the argc from main function
   * @param argv the argv from main function
   * @return 0 if a test was run, -1 if an error occured or no test found
   */
  static int ParseArgAndRun(int argc, char** argv) {
    if (argc < MIN_ARG_FOR_TEST) {
      // This function should not be called without enough arguments
      return -1;
    }
    // Check if we ONLY have "--test" & run SelfTest()
    if (argc == MIN_ARG_FOR_TEST && strcmp(argv[1], TEST_ARG) == 0) {
      SelfTest();
      return 0;
    }
    // Refactor needed if we add more tests
    // Check if we ONLY have "--test first-window"
    if (argc > MIN_ARG_FOR_TEST && strcmp(argv[1], TEST_ARG) == 0 &&
        strcmp(argv[2], FIRST_WINDOW_TEST_ARG) == 0) {
      return FirstWindowTest();
    }

    return -1;
  }

 private:
  /**
   * Run the self test to print "Hello tests"
   */
  static void SelfTest() { std::cout << "Hello tests" << std::endl; }

  /**
   * Run the first window test to print "Hello first window"
   */
  static int FirstWindowTest() {
    std::cout << "Hello first window" << std::endl;
    return 0;
  }
};
