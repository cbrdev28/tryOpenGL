#include "IndexBuffer.h"

#include <openGLErrorHelpers.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, unsigned int glDrawMode) : count_(count) {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, glDrawMode));
}

IndexBuffer::~IndexBuffer() {
  unBind();
  GLCall(glDeleteBuffers(1, &identifier_));
}

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_)); }

void IndexBuffer::unBind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

void IndexBuffer::setData(const void* data, unsigned int count) {
  // Note: 0 is the start offset (we always set the full set of data)
  GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
}