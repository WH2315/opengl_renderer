#pragma once

#include "resources/vertex_buffer.hpp"
#include "resources/index_buffer.hpp"
#include <memory>

namespace wen {

class VertexArray final {
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void attachVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer);
    void attachIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer);

private:
    uint32_t VAO_;
    GLuint location_ = 0;
};

} // namespace wen