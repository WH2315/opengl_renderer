#include "resources/vertex_buffer.hpp"

namespace wen {

VertexBuffer::VertexBuffer(uint32_t size) : size_(size) {
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &VBO_);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void* data, uint32_t offset) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size_, data);
}

void VertexBuffer::setVertexLayout(const VertexLayout layout) {
    layout_ = std::move(layout);
}

} // namespace wen