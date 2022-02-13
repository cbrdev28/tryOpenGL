/**
 * Window manager for glfw
 */

#include "WindowManager.h"

#include <fmt/core.h>

WindowManager::WindowManager() { fmt::print("WindowManager::WindowManager()\n"); }

int WindowManager::init() {
  fmt::print("WindowManager::init()\n");

  int initialized = glfwInit();
  if (initialized != GLFW_TRUE) {
    fmt::print("Failed to glfwInit()\n");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // For Apple
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(1024, 768, "WindowManager", NULL, NULL);
  if (window == NULL) {
    fmt::print("Failed to glfwCreateWindow(...)\n");
    glfwTerminate();
    return -1;
  }
  // TODO: store window
  //   glfwMakeContextCurrent(window);

  //   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  //     fmt::print("Failed to gladLoadGLLoader(...)\n");
  //     glfwTerminate();
  //     return -1;
  //   }
  return 0;
}