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

auto Renderer::drawInstance(ShaderManager& shaders, VertexArray& va, GLsizei count, GLsizei instancesCount)
    -> Renderer& {
  shaders.bind();
  va.bind();
  GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, count, instancesCount));
  va.unBind();
  shaders.unBind();
  return *this;
}