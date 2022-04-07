#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include <utility>
#include <vector>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
 private:
  GLuint identifier_{0};

 public:
  VertexArray();
  ~VertexArray();

  VertexArray(const VertexArray& other) = delete;
  VertexArray(VertexArray&& other) = delete;
  auto operator=(const VertexArray& other) -> VertexArray& = delete;
  auto operator=(VertexArray&& other) -> VertexArray& = delete;

  void setBufferLayout(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

  void setInstanceBufferLayout(
      const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>>& vectorOfPairs) const;

  void bind() const;
  void unBind() const;
};

#endif