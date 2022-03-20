#ifndef VERTEX_BUFFER_LAYOUT_H_
#define VERTEX_BUFFER_LAYOUT_H_

#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>

#include <vector>

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
    ASSERT(false)
    return 0;
  }
};

class VertexBufferLayout {
 private:
  std::vector<VertexBufferElement> elements_{};
  GLsizei stride_{0};

 public:
  VertexBufferLayout() = default;

  inline void pushFloat(GLint count) {
    elements_.push_back({GL_FLOAT, count, GL_FALSE});
    stride_ = stride_ + VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
  }

  inline void pushUInt(GLint count) {
    elements_.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride_ = stride_ + VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
  }

  [[nodiscard]] inline auto getElements() const -> const std::vector<VertexBufferElement>& { return elements_; }
  [[nodiscard]] inline auto getStride() const -> GLsizei { return stride_; }
};

#endif