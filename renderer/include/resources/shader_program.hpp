#pragma once

#include "resources/shader.hpp"
#include <memory>

namespace wen {

class ShaderProgram final {
public:
    ShaderProgram();
    ~ShaderProgram();

    ShaderProgram& attach(const std::shared_ptr<Shader>& shader);
    ShaderProgram& link();

    void bind() const;
    void unbind() const;

private:
    void checkLinkErrors(uint32_t program);

private:
    uint32_t program_;
};

} // namespace wen