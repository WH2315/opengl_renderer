#include "resources/vertex_array.hpp"
#include "core/log.hpp"

namespace wen {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &VAO_);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &VAO_);
}

void VertexArray::bind() const {
    glBindVertexArray(VAO_);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::attachVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) {
    if (vertex_buffer->layout_.elements_.size() == 0) {
        WEN_ERROR("you don't set layout for vertex buffer!")
    }

    this->bind();
    vertex_buffer->bind();

    GLuint location = 0;
    const auto& layout = vertex_buffer->layout_;
    for (const auto& element : layout) {
        switch (element.type) {
            case GL_FLOAT: {
                glVertexAttribPointer(
                    location,
                    element.size / 4,
                    element.type,
                    element.normalized ? GL_TRUE : GL_FALSE,
                    layout.stride_,
                    reinterpret_cast<const void*>(element.offset)
                );
                break;
            }
            case GL_INT: {
                glVertexAttribIPointer(
                    location,
                    element.size / 4,
                    element.type,
                    layout.stride_,
                    reinterpret_cast<const void*>(element.offset)
                );
                break;
            }
            case GL_BOOL: {
                glVertexAttribIPointer(
                    location,
                    element.size,
                    element.type,
                    layout.stride_,
                    reinterpret_cast<const void*>(element.offset)
                );
                break;
            }
            default: {
                WEN_ERROR("unknown vertex type!")
                break;
            }
        }
        glEnableVertexAttribArray(location);
        location++;
    }

    vertex_buffer->unbind();
    this->unbind();
}

void VertexArray::attachIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) {
    this->bind();
    index_buffer->bind();
    this->unbind();
}

} // namespace wen