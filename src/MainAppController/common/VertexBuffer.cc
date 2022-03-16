#include "VertexBuffer.h"

#include <openGLErrorHelpers.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int glDrawMode) {
  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, glDrawMode));
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &identifier_)); }

void VertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_)); }

void VertexBuffer::unBind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void VertexBuffer::setData(const void* data, unsigned int size) {
  // Note: 0 is the start offset (we always set the full set of data)
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}