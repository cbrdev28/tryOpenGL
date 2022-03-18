#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

#include <openGLHeaders.h>

class IndexBuffer {
 private:
  unsigned int identifier_{0};
  unsigned int count_{0};

 public:
  IndexBuffer(const unsigned int* data, unsigned int count, unsigned int glDrawMode = GL_STATIC_DRAW);
  ~IndexBuffer();

  IndexBuffer(const IndexBuffer& other) = delete;
  IndexBuffer(IndexBuffer&& other) = delete;
  auto operator=(const IndexBuffer& other) -> IndexBuffer& = delete;
  auto operator=(IndexBuffer&& other) -> IndexBuffer& = delete;

  void bind() const;
  void unBind() const;

  void setData(const void* data, unsigned int count);

  [[nodiscard]] inline auto getCount() const -> unsigned int { return count_; }
};

#endif