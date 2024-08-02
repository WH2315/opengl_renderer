#include "renderer.hpp"

namespace wen {

Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {}

void Renderer::setClearColor(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::bindResources(const std::shared_ptr<ShaderProgram>& shader_program, const std::shared_ptr<VertexArray>& vertex_array) const {
    shader_program->bind();
    if (vertex_array != nullptr) {
        vertex_array->bind();
    } 
}

void Renderer::bindTexture2D(const std::shared_ptr<Texture2D>& texture, uint32_t slot) const {
    texture->bind(slot);
}

void Renderer::draw(uint32_t vertex_count) const {
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

void Renderer::drawIndexed(IndexType type, uint32_t index_count) const {
    glDrawElements(GL_TRIANGLES, index_count, convert<GLenum>(type), nullptr);
}

void Renderer::drawModel(const std::shared_ptr<Model>& model, const std::shared_ptr<ShaderProgram>& shader_program, uint32_t count) const {
    model->draw(shader_program, count);
}

void Renderer::unbindResources(const std::shared_ptr<ShaderProgram>& shader_program, const std::shared_ptr<VertexArray>& vertex_array) const {
    vertex_array->unbind();
    if (vertex_array != nullptr) {
        vertex_array->unbind();
    } 
}

} // namespace wen