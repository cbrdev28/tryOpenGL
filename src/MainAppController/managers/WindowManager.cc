/**
 * Window manager for glfw
 */

#include "WindowManager.h"

#include <fmt/core.h>
#include <fmt/format.h>

// Initialize static class variables
int WindowManager::width = WindowManager::defaultWidth;
int WindowManager::height = WindowManager::defaultHeight;

WindowManager::WindowManager() : _window(nullptr) { fmt::print("WindowManager::WindowManager()\n"); }

WindowManager::~WindowManager() {
  fmt::print("WindowManager::~WindowManager()\n");
  if (_window != nullptr) {
    fmt::print("WindowManager::~WindowManager(): glfwTerminate()...\n");
    glfwTerminate();
    _window = nullptr;
  }
}

/**
 * Public
 */
WindowManager* WindowManager::init() {
  fmt::print("WindowManager::init()\n");

  int initialized = glfwInit();
  if (initialized != GLFW_TRUE) {
    fmt::print("Failed to glfwInit()\n");
    throw -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // For Apple
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window =
      glfwCreateWindow(WindowManager::defaultWidth, WindowManager::defaultHeight, "WindowManager", NULL, NULL);
  if (window == NULL) {
    fmt::print("Failed to glfwCreateWindow(...)\n");
    glfwTerminate();
    throw -1;
  }
  this->_window = window;
  glfwMakeContextCurrent(this->_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fmt::print("Failed to gladLoadGLLoader(...)\n");
    glfwTerminate();
    throw -1;
  }

  GLFWframebuffersizefun callback = glfwSetFramebufferSizeCallback(this->_window, framebufferSizeCallback);
  if (callback == NULL) {
    // Only warning for now, since it seems to be working anyway
    auto formattedPointer = fmt::ptr(callback);
    fmt::print("Warning glfwSetFramebufferSizeCallback(...): {}\n", formattedPointer);
  }
  return this;
}

/**
 * Private
 */

/**
 * Callback
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);

  WindowManager::width = width;
  WindowManager::height = height;
  // Debug: to be tested
  // fmt::print("framebufferSizeCallback w/ = {}, h = {}", width, height);
}
