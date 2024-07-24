#include "renderer.hpp"

namespace wen {

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::setClearColor(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::bindShaderProgram(const std::shared_ptr<ShaderProgram>& shader_program) const {
    shader_program->bind();
}

void Renderer::bindVertexArray(const std::shared_ptr<VertexArray>& vertex_array) const {
    vertex_array->bind();
}

void Renderer::draw(uint32_t vertex_count) const {
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

} // namespace wen