#include "VertexBuffer.h"

#include "openGLErrorHelpers.h"

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum glDrawMode) {
  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, glDrawMode));
}

VertexBuffer::~VertexBuffer() {
  unBind();
  GLCall(glDeleteBuffers(1, &identifier_));
}

void VertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_)); }

void VertexBuffer::unBind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void VertexBuffer::setData(const GLvoid* data, GLsizeiptr size) {
  // Note: 0 is the start offset (we always set the full set of data)
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void VertexBuffer::setInstanceData(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize) {
  bind();
  GLCall(glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_STREAM_DRAW));
  // Note: 0 is the start offset (we always set the full set of data)
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void VertexBuffer::setInstanceDataOffset(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize, GLintptr offset) {
  bind();
  GLCall(glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_STREAM_DRAW));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}