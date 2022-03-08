#ifndef OPEN_GL_ERROR_HELPERS_H_
#define OPEN_GL_ERROR_HELPERS_H_

/**
 * GL Error helpers
 */
#include <cassert>

#define ASSERT(x) assert(x);

#define GLCall(x) \
  clearError();   \
  x;              \
  ASSERT(checkAndLogError(#x, __FILE__, __LINE__));

void clearError();
auto checkAndLogError(const char* function, const char* file, int line) -> bool;

#endif