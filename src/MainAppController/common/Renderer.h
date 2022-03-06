#ifndef RENDERER_H_
#define RENDERER_H_

#include <cassert>

#include "openGLHeaders.h"

#define ASSERT(x) assert(x);

#define GLCall(x) \
  clearError();   \
  x;              \
  ASSERT(checkAndLogError(#x, __FILE__, __LINE__));

void clearError();
auto checkAndLogError(const char* function, const char* file, int line) -> bool;

#endif