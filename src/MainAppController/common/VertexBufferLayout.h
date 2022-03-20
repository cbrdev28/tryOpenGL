#ifndef VERTEX_BUFFER_LAYOUT_H_
#define VERTEX_BUFFER_LAYOUT_H_

#include <vector>

#include "VertexBuffer.h"
#include "VertexBufferElement.h"
#include "openGLErrorHelpers.h"
#include "openGLHeaders.h"

class VertexBufferLayout {
 private:
  std::vector<VertexBufferElement> elements_{};
  GLsizei stride_{0};

 public:
  VertexBufferLayout() = default;

  inline void pushFloat(GLint count, const VertexBuffer& vb) {
    elements_.push_back({GL_FLOAT, count, GL_FALSE, vb});
    stride_ = stride_ + VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
  }

  inline void pushUInt(GLint count, const VertexBuffer& vb) {
    elements_.push_back({GL_UNSIGNED_INT, count, GL_FALSE, vb});
    stride_ = stride_ + VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
  }

  [[nodiscard]] inline auto getElements() const -> const std::vector<VertexBufferElement>& { return elements_; }
  [[nodiscard]] inline auto getStride() const -> GLsizei { return stride_; }
};

#endif