#pragma once

#include "base/enums.hpp"
#include <cstdint>

namespace wen {

class IndexBuffer final {
public:
    IndexBuffer(IndexType type, uint32_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    void setData(const void* data, uint32_t offset = 0);

private:
    uint32_t IBO_;
    uint32_t size_;
};

} // namespace wen