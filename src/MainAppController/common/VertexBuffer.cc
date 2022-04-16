#include "cbrpch.h"

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size) {
  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  unBind();
  GLCall(glDeleteBuffers(1, &identifier_));
}

void VertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_)); }

void VertexBuffer::unBind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
