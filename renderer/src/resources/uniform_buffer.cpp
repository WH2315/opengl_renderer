#include "resources/uniform_buffer.hpp"
#include <glad/glad.h>

namespace wen {

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) {
    glCreateBuffers(1, &UBO_);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, UBO_);
    glNamedBufferData(UBO_, size, nullptr, GL_DYNAMIC_DRAW);
}

UniformBuffer::~UniformBuffer() {
    glDeleteBuffers(1, &UBO_);
}

void UniformBuffer::setData(const void* data, uint32_t size, uint32_t offset) {
    glNamedBufferSubData(UBO_, offset, size, data);
}

} // namespace wen