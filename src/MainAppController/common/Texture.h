#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include "openGLHeaders.h"

class Texture {
 private:
  GLuint identifier_{0};
  std::string filePath_;
  unsigned char* localBuffer_{nullptr};
  GLsizei width_{0}, height_{0};
  int channelsInFile_{0};

 public:
  explicit Texture(std::string filePath);
  ~Texture();

  Texture(const Texture& other) = delete;
  Texture(Texture&& other) = delete;
  auto operator=(const Texture& other) -> Texture& = delete;
  auto operator=(Texture&& other) -> Texture& = delete;

  void bind(unsigned int slot = 0) const;
  void unBind() const;
};

#endif