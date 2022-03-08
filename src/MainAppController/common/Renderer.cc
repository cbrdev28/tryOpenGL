#include "Renderer.h"

#include <basicBackgroundColor.h>

/**
 * Renderer class
 */
auto Renderer::clear() -> Renderer& {
  GLCall(glClearColor(basicBackgroundNeonPinkR, basicBackgroundNeonPinkG, basicBackgroundNeonPinkB, 1.0F));
  GLCall(glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/));
  return *this;
}

auto Renderer::draw(const ShaderManager& shaders, const VertexArray& va, const IndexBuffer& ib) -> Renderer& {
  shaders.bind();
  va.bind();
  ib.bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
  return *this;
}