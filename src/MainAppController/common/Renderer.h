#ifndef RENDERER_H_
#define RENDERER_H_

#include "IndexBuffer.h"
#include "ShaderManager.h"
#include "VertexArray.h"

/**
 * Renderer class to issue OpenGL draw calls
 */
class Renderer {
 public:
  auto clear() -> Renderer&;
  auto clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) -> Renderer&;
  auto draw(ShaderManager& shaders, VertexArray& va, IndexBuffer& ib) -> Renderer&;
  auto drawInstance(ShaderManager& shaders, VertexArray& va, IndexBuffer& ib, GLsizei instancesCount) -> Renderer&;
};

#endif