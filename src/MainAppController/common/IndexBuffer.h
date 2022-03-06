#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

class IndexBuffer {
 private:
  unsigned int identifier_{0};
  unsigned int count_{0};

 public:
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  IndexBuffer(const IndexBuffer& other) = delete;
  IndexBuffer(IndexBuffer&& other) = delete;
  auto operator=(const IndexBuffer& other) -> IndexBuffer& = delete;
  auto operator=(IndexBuffer&& other) -> IndexBuffer& = delete;

  void bind() const;
  void unBind() const;

  [[nodiscard]] inline auto getCount() const -> unsigned int { return count_; }
};

#endif