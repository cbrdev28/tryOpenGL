#ifndef RENDERER_H_
#define RENDERER_H_

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

/**
 * Renderer class to issue OpenGL draw calls
 */
class Renderer {
 public:
  void clear();
  void clearColorBackground(std::array<float, 4>& backgroundColor);
  void clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

  void draw(Shader& shaders, VertexArray& va, IndexBuffer& ib);
  void draw(Shader& shaders, VertexArray& va, unsigned int count);
  void drawInstance(Shader& shaders, VertexArray& va, GLsizei count, GLsizei instancesCount);

  void enableBlend();
};

#endif