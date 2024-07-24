#include "resources/index_buffer.hpp"
#include "base/utils.hpp"

namespace wen {

IndexBuffer::IndexBuffer(IndexType type, uint32_t count) : size_(count * convert<size_t>(type)) {
    glGenBuffers(1, &IBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, nullptr, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &IBO_);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::setData(const void* data, uint32_t offset) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size_, data);
}

} // namespace wen