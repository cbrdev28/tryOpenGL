#include "openGLErrorHelpers.h"

#include <fmt/core.h>

#include "openGLHeaders.h"

/**
 * GL Error helpers
 */
void clearError() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

auto checkAndLogError(const char* function, const char* file, int line) -> bool {
  while (GLenum error = glGetError()) {
    fmt::print("OpenGL ERROR {}: {}: {}: {}\n", error, function, file, line);
    return false;
  }
  return true;
}
