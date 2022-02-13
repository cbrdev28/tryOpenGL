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

  GLFWerrorfun errorCallback = glfwSetErrorCallback(WindowManager::errorCallback);
  if (errorCallback == NULL) {
    // Only warning
    auto formattedPointer = fmt::ptr(errorCallback);
    fmt::print("Warning glfwSetErrorCallback(...): {}\n", formattedPointer);
  }

  GLFWwindow* window =
      glfwCreateWindow(WindowManager::defaultWidth, WindowManager::defaultHeight, "WindowManager", NULL, NULL);
  if (window == NULL) {
    fmt::print("Failed to glfwCreateWindow(...)\n");
    throw -1;
  }
  this->_window = window;
  glfwMakeContextCurrent(this->_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fmt::print("Failed to gladLoadGLLoader(...)\n");
    throw -1;
  }

  GLFWframebuffersizefun fsCallback =
      glfwSetFramebufferSizeCallback(this->_window, WindowManager::framebufferSizeCallback);
  if (fsCallback == NULL) {
    // Only warning for now, since it seems to be working anyway
    auto formattedPointer = fmt::ptr(fsCallback);
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
void WindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);

  WindowManager::width = width;
  WindowManager::height = height;
  // Debug: to be tested
  // fmt::print("framebufferSizeCallback w/ = {}, h = {}", width, height);
}

void WindowManager::errorCallback(int code, const char* description) {
  fmt::print("WindowManager::errorCallback(...): code = {} / description = {}\n", code, description);
}
