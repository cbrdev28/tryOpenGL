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
  auto clear() -> Renderer&;
  auto clearColorBackground(std::array<float, 4>& backgroundColor) -> Renderer&;
  auto clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) -> Renderer&;

  auto draw(Shader& shaders, VertexArray& va, IndexBuffer& ib) -> Renderer&;
  auto draw(Shader& shaders, VertexArray& va, unsigned int count) -> Renderer&;
  auto drawInstance(Shader& shaders, VertexArray& va, GLsizei count, GLsizei instancesCount) -> Renderer&;

  void enableBlend();
};

#endif