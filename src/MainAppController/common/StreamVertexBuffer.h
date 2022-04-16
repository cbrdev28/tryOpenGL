#ifndef STREAM_VERTEX_BUFFER_H_
#define STREAM_VERTEX_BUFFER_H_

#include "VertexBufferElement.h"
#include "VertexBufferLayout.h"

enum VBSUsage {
  ONCE = GLuint(0),  // Value matching to use with `glVertexAttribDivisor`
  EACH = GLuint(1),
};

class StreamVertexBuffer {
 private:
  GLuint identifier_{0};
  GLuint usageDivisor_;
  VertexBufferLayout layout_;

 public:
  StreamVertexBuffer(const GLvoid* data, GLsizeiptr size, const VertexBufferElement& vbElement,
                     VBSUsage usageDivisor = EACH, GLenum glDrawMode = GL_STREAM_DRAW);
  ~StreamVertexBuffer();
  StreamVertexBuffer(const StreamVertexBuffer& other) = delete;
  StreamVertexBuffer(StreamVertexBuffer&& other) = delete;
  auto operator=(const StreamVertexBuffer& other) -> StreamVertexBuffer& = delete;
  auto operator=(StreamVertexBuffer&& other) -> StreamVertexBuffer& = delete;

  void bind() const;
  void unBind() const;

  [[deprecated("Prefer to use version which always take the offset as parameter")]] void setInstanceData(
      const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize) const;
  void setInstanceDataOffset(const GLvoid* data, GLsizeiptr size, GLsizeiptr maxSize, GLintptr offset) const;

  [[nodiscard]] inline auto getUsage() const -> GLuint { return usageDivisor_; }
  [[nodiscard]] inline auto getLayout() const -> const VertexBufferLayout& { return layout_; }
};

#endif