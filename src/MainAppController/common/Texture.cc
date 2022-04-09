#include "cbrpch.h"

#include "Texture.h"

#include "openGLErrorHelpers.h"

Texture::Texture(std::string filePath) : filePath_(std::move(filePath)) {
  stbi_set_flip_vertically_on_load(1);
  localBuffer_ = stbi_load(filePath_.c_str(), &width_, &height_, &channelsInFile_, STBI_rgb_alpha);
  if (localBuffer_ == nullptr) {
    fmt::print("Failed to open texture file: {}\n", filePath_);
    throw -1;
  }

  GLCall(glGenTextures(1, &identifier_));
  GLCall(glBindTexture(GL_TEXTURE_2D, identifier_));

  // LearnOpenGL:
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Cherno:
  // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer_));
  // LearnOpenGL:
  // glGenerateMipmap(GL_TEXTURE_2D);
  unBind();

  if (localBuffer_ != nullptr) {
    stbi_image_free(localBuffer_);
    localBuffer_ = nullptr;
  }
}

Texture::~Texture() {
  unBind();
  GLCall(glDeleteTextures(1, &identifier_));

  if (localBuffer_ != nullptr) {
    stbi_image_free(localBuffer_);
    localBuffer_ = nullptr;
  }
}

void Texture::bind(unsigned int slot /* = 0 */) const {
  // Hardcoded max slot supported
  ASSERT(slot < 8);

  switch (slot) {
    case 0:
      GLCall(glActiveTexture(GL_TEXTURE0));
      break;
    case 1:
      GLCall(glActiveTexture(GL_TEXTURE1));
      break;
    case 2:
      GLCall(glActiveTexture(GL_TEXTURE2));
      break;
    case 3:
      GLCall(glActiveTexture(GL_TEXTURE3));
      break;
    case 4:
      GLCall(glActiveTexture(GL_TEXTURE4));
      break;
    case 5:
      GLCall(glActiveTexture(GL_TEXTURE5));
      break;
    case 6:
      GLCall(glActiveTexture(GL_TEXTURE6));
      break;
    case 7:
      GLCall(glActiveTexture(GL_TEXTURE7));
      break;
  }
  GLCall(glBindTexture(GL_TEXTURE_2D, identifier_));
}

void Texture::unBind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
