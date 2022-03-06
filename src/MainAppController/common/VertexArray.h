#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include <VertexBuffer.h>
#include <VertexBufferLayout.h>

class VertexArray {
 private:
  unsigned int identifier_{0};

 public:
  VertexArray();
  ~VertexArray();

  VertexArray(const VertexArray& other) = delete;
  VertexArray(VertexArray&& other) = delete;
  auto operator=(const VertexArray& other) -> VertexArray& = delete;
  auto operator=(VertexArray&& other) -> VertexArray& = delete;

  void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

  void bind() const;
  void unBind() const;
};

#endif