#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include "openGLHeaders.h"

class VertexBuffer {
 private:
  GLuint identifier_{0};

 public:
  VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum glDrawMode = GL_STATIC_DRAW);
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer& other) = delete;
  VertexBuffer(VertexBuffer&& other) = delete;
  auto operator=(const VertexBuffer& other) -> VertexBuffer& = delete;
  auto operator=(VertexBuffer&& other) -> VertexBuffer& = delete;

  void bind() const;
  void unBind() const;

  void setData(const GLvoid* data, GLsizeiptr size);
  void setInstanceData(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize);
};

#endif