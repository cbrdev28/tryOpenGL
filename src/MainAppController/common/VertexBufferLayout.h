#ifndef VERTEX_BUFFER_LAYOUT_H_
#define VERTEX_BUFFER_LAYOUT_H_

#include <Renderer.h>

#include <vector>

struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static auto getSizeOfType(unsigned int type) -> unsigned int {
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
  unsigned int stride_{0};

 public:
  VertexBufferLayout() = default;

  void pushFloat(unsigned int count) {
    elements_.push_back({GL_FLOAT, count, static_cast<unsigned char>(GL_FALSE)});
    stride_ = stride_ + VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
  }

  void pushUInt(unsigned int count) {
    elements_.push_back({GL_UNSIGNED_INT, count, static_cast<unsigned char>(GL_FALSE)});
    stride_ = stride_ + VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
  }

  [[nodiscard]] inline auto getElements() const -> std::vector<VertexBufferElement> { return elements_; }
  [[nodiscard]] inline auto getStride() const -> unsigned int { return stride_; }
};

#endif