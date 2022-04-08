#include "cbrpch.h"

#include "Renderer.h"

#include "openGLErrorHelpers.h"

/**
 * Renderer class
 */
auto Renderer::clear() -> Renderer& {
  GLCall(glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/));
  return *this;
}

auto Renderer::clearColorBackground(std::array<float, 4>& backgroundColor) -> Renderer& {
  this->clearColorBackground(backgroundColor.at(0), backgroundColor.at(1), backgroundColor.at(2),
                             backgroundColor.at(3));
  return *this;
}

auto Renderer::clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) -> Renderer& {
  GLCall(glClearColor(red, green, blue, alpha));
  return *this;
}

auto Renderer::draw(Shader& shaders, VertexArray& va, IndexBuffer& ib) -> Renderer& {
  shaders.bind();
  va.bind();
  ib.bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
  ib.unBind();
  va.unBind();
  shaders.unBind();
  return *this;
}

auto Renderer::draw(Shader& shaders, VertexArray& va, unsigned int count) -> Renderer& {
  shaders.bind();
  va.bind();
  GLCall(glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count)));
  va.unBind();
  shaders.unBind();
  return *this;
}

auto Renderer::drawInstance(Shader& shaders, VertexArray& va, GLsizei count, GLsizei instancesCount) -> Renderer& {
  shaders.bind();
  va.bind();
  GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, count, instancesCount));
  va.unBind();
  shaders.unBind();
  return *this;
}