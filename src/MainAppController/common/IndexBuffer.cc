#include "IndexBuffer.h"

#include "openGLErrorHelpers.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLsizei count, GLenum glDrawMode) : data_(data), count_(count) {
  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, glDrawMode));
}

IndexBuffer::~IndexBuffer() {
  unBind();
  GLCall(glDeleteBuffers(1, &identifier_));
}

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_)); }

void IndexBuffer::unBind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

void IndexBuffer::setData(const GLvoid* data, GLsizei count) {
  // Note: 0 is the start offset (we always set the full set of data)
  // Refactor: this may need to call `glBufferData` first (see VertexBuffer)
  GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(GLuint), data));
}