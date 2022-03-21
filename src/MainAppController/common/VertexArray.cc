#include "VertexArray.h"

#include "VertexBuffer.h"
#include "openGLErrorHelpers.h"
#include "openGLHeaders.h"

VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &identifier_)); }

VertexArray::~VertexArray() {
  unBind();
  GLCall(glDeleteVertexArrays(1, &identifier_));
}

void VertexArray::setBufferLayout(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
  bind();
  vb.bind();
  uintptr_t offset = 0;
  const auto& elements = layout.getElements();
  for (GLuint i = 0; i < elements.size(); i++) {
    // NOLINTNEXTLINE(google-readability-casting, performance-no-int-to-ptr, cppcoreguidelines-pro-type-cstyle-cast)
    auto* glOffset = (GLvoid*)offset;
    const auto& element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), glOffset));
    offset = offset + static_cast<uint64_t>(element.count * VertexBufferElement::getSizeOfType(element.type));
  }
}

void VertexArray::setInstanceBufferElement(
    const std::vector<std::pair<const VertexBuffer&, const VertexBufferElement&>>& bufferElement) const {
  bind();
  for (GLuint i = 0; i < bufferElement.size(); i++) {
    const auto& element = bufferElement.at(i).second;
    const auto& buffer = bufferElement.at(i).first;
    buffer.bind();
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                 element.count * VertexBufferElement::getSizeOfType(element.type), nullptr));
    GLCall(glVertexAttribDivisor(i, buffer.getDivisor()));
  }
}

void VertexArray::bind() const { GLCall(glBindVertexArray(identifier_)); }

void VertexArray::unBind() const { GLCall(glBindVertexArray(0)); }