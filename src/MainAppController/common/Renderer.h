#ifndef RENDERER_H_
#define RENDERER_H_

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "openGLHeaders.h"

/**
 * Renderer class to issue OpenGL draw calls
 */
class Renderer {
 public:
  auto clear() -> Renderer&;
  auto clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) -> Renderer&;

  auto draw(Shader& shaders, VertexArray& va, IndexBuffer& ib) -> Renderer&;
  auto drawInstance(Shader& shaders, VertexArray& va, GLsizei count, GLsizei instancesCount) -> Renderer&;
};

#endif