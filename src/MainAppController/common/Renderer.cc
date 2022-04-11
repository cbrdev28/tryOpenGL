#include "cbrpch.h"

#include "Renderer.h"

/**
 * Renderer class
 */
void Renderer::clear() { GLCall(glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/)); }

void Renderer::clearColorBackground(std::array<float, 4>& backgroundColor) {
  this->clearColorBackground(backgroundColor.at(0), backgroundColor.at(1), backgroundColor.at(2),
                             backgroundColor.at(3));
}

void Renderer::clearColorBackground(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
  GLCall(glClearColor(red, green, blue, alpha));
}

void Renderer::draw(Shader& shaders, VertexArray& va, IndexBuffer& ib) {
  shaders.bind();
  va.bind();
  ib.bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
  ib.unBind();
  va.unBind();
  shaders.unBind();
}

void Renderer::draw(Shader& shaders, VertexArray& va, unsigned int count) {
  shaders.bind();
  va.bind();
  GLCall(glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count)));
  va.unBind();
  shaders.unBind();
}

void Renderer::drawInstance(Shader& shaders, VertexArray& va, GLsizei count, GLsizei instancesCount) {
  shaders.bind();
  va.bind();
  GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, count, instancesCount));
  va.unBind();
  shaders.unBind();
}

void Renderer::enableBlend() {
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
