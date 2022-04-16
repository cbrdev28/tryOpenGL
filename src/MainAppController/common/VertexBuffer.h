#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

class VertexBuffer {
 private:
  GLuint identifier_{0};

 public:
  VertexBuffer(const GLvoid* data, GLsizeiptr size);
  ~VertexBuffer();
  VertexBuffer(const VertexBuffer& other) = delete;
  VertexBuffer(VertexBuffer&& other) = delete;
  auto operator=(const VertexBuffer& other) -> VertexBuffer& = delete;
  auto operator=(VertexBuffer&& other) -> VertexBuffer& = delete;

  void bind() const;
  void unBind() const;
};

#endif