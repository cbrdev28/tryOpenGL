#include "Texture.h"

#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>
#include <stb_image.h>

Texture::Texture(std::string filePath) : filePath_(std::move(filePath)) {
  stbi_set_flip_vertically_on_load(1);
  localBuffer_ = stbi_load(filePath_.c_str(), &width_, &height_, &bpp_, STBI_rgb_alpha);
  ASSERT(localBuffer_ != nullptr);

  GLCall(glGenTextures(1, &identifier_));
  GLCall(glBindTexture(GL_TEXTURE_2D, identifier_));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer_));
  unBind();

  if (localBuffer_ != nullptr) {
    stbi_image_free(localBuffer_);
    localBuffer_ = nullptr;
  }
}

Texture::~Texture() {
  GLCall(glDeleteTextures(1, &identifier_));

  if (localBuffer_ != nullptr) {
    stbi_image_free(localBuffer_);
    localBuffer_ = nullptr;
  }
}

void Texture::bind(unsigned int slot /* = 0 */) const {
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, identifier_));
}

void Texture::unBind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
