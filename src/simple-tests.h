/**
 * This file is meant to do a simple parsing of the args
 * to check if we have:
 * - "--test": to print the self test "Hello tests"
 * - TODO: "--test first-window": to play with glfw, opengl, ...
 * - TODO: and more stuff that I will forget to document
 */

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string.h>

#include <chrono>
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
    int initialized = glfwInit();
    if (initialized != GLFW_TRUE) {
      std::cout << "Failed to glfwInit" << std::endl;
      return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For Apple
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "FirstWindow", NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to glfwCreateWindow" << std::endl;
      glfwTerminate();
      return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to gladLoadGLLoader" << std::endl;
      glfwTerminate();
      return -1;
    }

    glViewport(0, 0, 800, 600);
    // TODO: Add callback: framebuffer_size_callback

    auto start = std::chrono::system_clock::now();
    std::cout << "Hello first window" << std::endl;
    auto end = std::chrono::system_clock::now();
    auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    while (!glfwWindowShouldClose(window) && elapsed_seconds < 3) {
      glClearColor(0.3f, 0.4f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      end = std::chrono::system_clock::now();
      elapsed_seconds =
          std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwTerminate();
    return 0;
  }
};
