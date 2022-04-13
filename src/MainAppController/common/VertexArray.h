#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include "StreamVertexBuffer.h"
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

  [[deprecated("Replaced by the version which takes a vector of pairs")]] void setBufferLayout(
      const VertexBuffer& vb, const VertexBufferLayout& layout) const;

  [[deprecated("Try to use version with VexterBufferStream")]] void setInstanceBufferLayout(
      const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>>& vectorOfPairs) const;
  void setBufferLayout(
      const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>>& vectorOfPairs) const;

  void setStreamBufferLayout(const std::vector<const StreamVertexBuffer*>& streamVBs) const;

  void bind() const;
  void unBind() const;
};

#endif