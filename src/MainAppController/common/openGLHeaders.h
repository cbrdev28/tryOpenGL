#ifndef OPEN_GL_HEADERS_H
#define OPEN_GL_HEADERS_H

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <fmt/core.h>

#define ASSERT(x) assert(x);
#define GLCall(x) \
  clearError();   \
  x;              \
  ASSERT(checkAndLogError(#x, __FILE__, __LINE__));

static void clearError() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

static auto checkAndLogError(const char* function, const char* file, int line) -> bool {
  while (GLenum error = glGetError()) {
    fmt::print("OpenGL ERROR {}: {}: {}: {}\n", error, function, file, line);
    return false;
  }
  return true;
}

#endif
