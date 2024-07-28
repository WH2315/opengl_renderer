#pragma once

#include "resources/shader_program.hpp"
#include "resources/vertex_array.hpp"
#include "resources/texture_2d.hpp"

namespace wen {

class Renderer final {
public:
    Renderer();
    ~Renderer();

    void setClearColor(float r, float g, float b, float a) const;
    void bindResources(const std::shared_ptr<ShaderProgram>& shader_program, const std::shared_ptr<VertexArray>& vertex_array) const;
    void bindTexture2D(const std::shared_ptr<Texture2D>& texture, uint32_t slot = 0) const;
    void draw(uint32_t vertex_count) const;
    void drawIndexed(IndexType type, uint32_t index_count) const;
};

} // namespace wen