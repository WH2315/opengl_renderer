#pragma once

#include <cstdint>

namespace wen {

class UniformBuffer final {
public:
    UniformBuffer(uint32_t size, uint32_t binding);
    ~UniformBuffer();

    void setData(const void* data, uint32_t size, uint32_t offset = 0);

private:
    uint32_t UBO_;
};

} // namespace wen