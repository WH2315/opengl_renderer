#pragma once

#include "resources/shader_program.hpp"
#include "resources/vertex_array.hpp"

namespace wen {

class Renderer final {
public:
    Renderer();
    ~Renderer();

    void setClearColor(float r, float g, float b, float a) const;
    void bindShaderProgram(const std::shared_ptr<ShaderProgram>& shader_program) const;
    void bindVertexArray(const std::shared_ptr<VertexArray>& vertex_array) const;
    void draw(uint32_t vertex_count) const;
};

} // namespace wen