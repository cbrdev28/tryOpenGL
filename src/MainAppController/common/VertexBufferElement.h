#ifndef VERTEX_BUFFER_ELEMENT_H_
#define VERTEX_BUFFER_ELEMENT_H_

#include "openGLErrorHelpers.h"
#include "openGLHeaders.h"

struct VertexBufferElement {
  GLenum type;
  GLint count;
  GLboolean normalized;

  inline static auto getSizeOfType(GLenum type) -> GLsizei {
    switch (type) {
      case GL_FLOAT:
        return sizeof(GLfloat);
      case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    }
    ASSERT(false);
    return 0;
  }
};

#endif
