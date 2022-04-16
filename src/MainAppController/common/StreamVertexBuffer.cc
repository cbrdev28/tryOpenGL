#include "cbrpch.h"

#include "StreamVertexBuffer.h"

#include "VertexBufferElement.h"

StreamVertexBuffer::StreamVertexBuffer(const GLvoid* data, GLsizeiptr size, const VertexBufferElement& vbElement,
                                       VBSUsage usageDivisor, GLenum glDrawMode)
    : usageDivisor_(usageDivisor) {
  // Set layout based on Vertext Buffer Element
  switch (vbElement.type) {
    case GL_FLOAT:
      layout_.pushFloat(vbElement.count);
      break;
    default:
      // We only support float (for now)
      // NOLINTNEXTLINE(misc-static-assert)
      ASSERT(false);
  }

  GLCall(glGenBuffers(1, &identifier_));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, glDrawMode));
}

StreamVertexBuffer::~StreamVertexBuffer() {
  unBind();
  GLCall(glDeleteBuffers(1, &identifier_));
}

void StreamVertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, identifier_)); }

void StreamVertexBuffer::unBind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void StreamVertexBuffer::setInstanceData(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize) const {
  bind();
  GLCall(glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_STREAM_DRAW));
  // Note: 0 is the start offset (we always set the full set of data)
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void StreamVertexBuffer::setInstanceDataOffset(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize,
                                               GLintptr offset) const {
  bind();
  // Experiment more and see if this is needed when adding more instances
  // if (offset == 0) {
  //   GLCall(glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_STREAM_DRAW));
  // }
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
