#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include "openGLHeaders.h"

enum VertexBufferDivisor {
  ALWAYS = GLuint(0),
  FOR_EACH = GLuint(1),
};

class VertexBuffer {
 private:
  GLuint identifier_{0};
  GLuint divisor_ = VertexBufferDivisor::ALWAYS;

 public:
  VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum glDrawMode = GL_STATIC_DRAW);
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer& other) = delete;
  VertexBuffer(VertexBuffer&& other) = delete;
  auto operator=(const VertexBuffer& other) -> VertexBuffer& = delete;
  auto operator=(VertexBuffer&& other) -> VertexBuffer& = delete;

  void bind() const;
  void unBind() const;

  // Experimental: used in TestDynamicRender but we prefer to use `setInstanceData` instead (in TestBackToBasic).
  void setData(const GLvoid* data, GLsizeiptr size);
  void setInstanceData(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize) const;
  void setInstanceDataOffset(const GLvoid* data, GLsizeiptr size, GLintptr offset) const;

  inline void setDivisor(const VertexBufferDivisor& divisor) { divisor_ = divisor; }
  [[nodiscard]] inline auto getDivisor() const -> GLuint { return divisor_; }
};

#endif