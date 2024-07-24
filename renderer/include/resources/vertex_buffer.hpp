#pragma once

#include "resources/vertex_layout.hpp"

namespace wen {

class VertexBuffer final {
    friend class VertexArray;

public:
    VertexBuffer(uint32_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void setData(const void* data, uint32_t offset = 0);
    void setVertexLayout(const VertexLayout layout);

private:
    uint32_t VBO_;
    uint32_t size_;
    VertexLayout layout_;
};

} // namespace wen