#include "VertexArray.h"

#include <openGLErrorHelpers.h>
#include <openGLHeaders.h>

VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &identifier_)); }

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &identifier_)); }

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
  bind();
  vb.bind();

  uintptr_t offset = 0;
  const auto& elements = layout.getElements();
  for (int i = 0; i < elements.size(); i++) {
    // NOLINTNEXTLINE(google-readability-casting, performance-no-int-to-ptr, cppcoreguidelines-pro-type-cstyle-cast)
    auto* glOffset = (GLvoid*)offset;
    const auto& element = elements[i];

    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), glOffset));

    offset = offset + static_cast<uint64_t>(element.count * VertexBufferElement::getSizeOfType(element.type));
  }
}

void VertexArray::bind() const { GLCall(glBindVertexArray(identifier_)); }

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void VertexArray::unBind() const { GLCall(glBindVertexArray(0)); }