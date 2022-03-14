#include "Renderer.h"

/**
 * Renderer class
 */
auto Renderer::clear() -> Renderer& {
  GLCall(glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/));
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