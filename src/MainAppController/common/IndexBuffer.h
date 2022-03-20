#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

#include "openGLHeaders.h"

class IndexBuffer {
 private:
  GLuint identifier_{0};
  const GLuint* data_{nullptr};
  GLsizei count_{0};

 public:
  IndexBuffer(const GLuint* data, GLsizei count, GLenum glDrawMode = GL_STATIC_DRAW);
  ~IndexBuffer();

  IndexBuffer(const IndexBuffer& other) = delete;
  IndexBuffer(IndexBuffer&& other) = delete;
  auto operator=(const IndexBuffer& other) -> IndexBuffer& = delete;
  auto operator=(IndexBuffer&& other) -> IndexBuffer& = delete;

  void bind() const;
  void unBind() const;

  void setData(const GLvoid* data, GLsizei count);

  [[nodiscard]] inline auto getCount() const -> GLsizei { return count_; }
  [[nodiscard]] inline auto getData() const -> const GLuint* { return data_; }
};

#endif