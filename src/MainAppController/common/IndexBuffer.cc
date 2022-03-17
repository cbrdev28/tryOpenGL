#include "IndexBuffer.h"

#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count_(count) {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  unBind();
  GLCall(glDeleteBuffers(1, &identifier_));
}

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_)); }

void IndexBuffer::unBind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }