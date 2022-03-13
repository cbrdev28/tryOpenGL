#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

class Texture {
 private:
  unsigned int identifier_{0};
  std::string filePath_;
  unsigned char* localBuffer_{nullptr};
  int width_{0}, height_{0}, bpp_{0};

 public:
  explicit Texture(std::string filePath);
  ~Texture();

  Texture(const Texture& other) = delete;
  Texture(Texture&& other) = delete;
  auto operator=(const Texture& other) -> Texture& = delete;
  auto operator=(Texture&& other) -> Texture& = delete;

  void bind(unsigned int slot = 0) const;
  void unBind() const;

  inline auto getWidth() -> int { return width_; }
  inline auto getHeight() -> int { return height_; }
};

#endif