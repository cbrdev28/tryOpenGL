#include "cbrpch.h"

#include "VertexArray.h"

#include "VertexBuffer.h"
#include "VertexBufferElement.h"

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

void VertexArray::setBufferLayout(
    const std::vector<std::pair<const VertexBuffer&, const VertexBufferLayout&>>& vectorOfPairs) const {
  bind();
  for (GLuint i = 0; i < vectorOfPairs.size(); i++) {
    const auto& aPair = vectorOfPairs.at(i);
    const auto& aBuffer = aPair.first;
    aBuffer.bind();
    const auto& aLayout = aPair.second;
    uint64_t offset = 0;
    for (GLuint j = 0; j < aLayout.getElements().size(); j++) {
      // NOLINTNEXTLINE(google-readability-casting, performance-no-int-to-ptr, cppcoreguidelines-pro-type-cstyle-cast)
      auto* glOffset = (GLvoid*)offset;
      const auto& element = aLayout.getElements().at(j);
      GLCall(glEnableVertexAttribArray(i + j));
      GLCall(
          glVertexAttribPointer(i + j, element.count, element.type, element.normalized, aLayout.getStride(), glOffset));
      offset = offset + static_cast<uint64_t>(element.count * VertexBufferElement::getSizeOfType(element.type));
    }
  }
}

void VertexArray::setStreamBufferLayout(const std::vector<const StreamVertexBuffer*>& streamVBs) const {
  bind();
  for (GLuint i = 0; i < streamVBs.size(); i++) {
    const auto& svb = streamVBs.at(i);
    svb->bind();

    uint64_t offset = 0;
    const auto& layout = svb->getLayout();
    for (GLuint j = 0; j < layout.getElements().size(); j++) {
      const auto& element = layout.getElements().at(j);

      // NOLINTNEXTLINE(google-readability-casting, performance-no-int-to-ptr, cppcoreguidelines-pro-type-cstyle-cast)
      auto* glOffset = (GLvoid*)offset;
      GLCall(glEnableVertexAttribArray(i + j));
      GLCall(
          glVertexAttribPointer(i + j, element.count, element.type, element.normalized, layout.getStride(), glOffset));
      GLCall(glVertexAttribDivisor(i + j, svb->getUsage()));
      offset = offset + static_cast<uint64_t>(element.count * VertexBufferElement::getSizeOfType(element.type));
    }
  }
}

void VertexArray::bind() const { GLCall(glBindVertexArray(identifier_)); }

void VertexArray::unBind() const { GLCall(glBindVertexArray(0)); }