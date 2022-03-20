#include "Renderer.h"

/**
 * Renderer class
 */
auto Renderer::clear() -> Renderer& {
  GLCall(glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/));
  return *this;
}

auto Renderer::clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) -> Renderer& {
  GLCall(glClearColor(red, green, blue, alpha));
  return *this;
}

auto Renderer::draw(ShaderManager& shaders, VertexArray& va, IndexBuffer& ib) -> Renderer& {
  shaders.bind();
  va.bind();
  ib.bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
  ib.unBind();
  va.unBind();
  shaders.unBind();
  return *this;
}

auto Renderer::drawInstance(ShaderManager& shaders, VertexArray& va, IndexBuffer& ib, GLsizei instancesCount)
    -> Renderer& {
  shaders.bind();
  va.bind();
  ib.bind();
  glVertexAttribDivisor(0, 0);  // Triangle vertices : always reuse the same 3 vertices -> 0
  glVertexAttribDivisor(1, 1);  // Positions : one per triangle -> 1
  GLCall(glDrawElementsInstanced(GL_TRIANGLE_STRIP, ib.getCount(), GL_UNSIGNED_INT, ib.getData(), instancesCount));
  ib.unBind();
  va.unBind();
  shaders.unBind();
  return *this;
}