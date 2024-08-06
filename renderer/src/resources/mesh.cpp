#include "resources/mesh.hpp"
#include "resources/vertex_buffer.hpp"
#include "resources/index_buffer.hpp"

namespace wen {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<ModelTexture> textures) {
    vertices_ = vertices;
    indices_ = indices;
    textures_ = textures;

    vao_ = std::make_shared<VertexArray>();
    auto vertex_buffer = std::make_shared<VertexBuffer>(vertices.size() * sizeof(Vertex));
    auto index_buffer = std::make_shared<IndexBuffer>(IndexType::eUInt32, indices.size());

    vertex_buffer->setData(vertices.data());
    index_buffer->setData(indices.data());

    vertex_buffer->setVertexLayout({
        {"position", VertexType::eFloat3},
        {"normal", VertexType::eFloat3},
        {"tex_coords", VertexType::eFloat2}
    });

    vao_->attachVertexBuffer(vertex_buffer);
    vao_->attachIndexBuffer(index_buffer);
}

void Mesh::draw(const std::shared_ptr<ShaderProgram>& program, uint32_t count) {
    uint32_t diffuse_number = 1;
    uint32_t specular_number = 1;
    for (uint32_t i = 0; i < textures_.size(); i++) {
        std::string number;
        std::string name = textures_[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuse_number++);
        } else if (name == "texture_specular") {
            number = std::to_string(specular_number++);
        }
        program->setInt((name + number).c_str(), i);
        glBindTextureUnit(i, textures_[i].texture);
    }

    // draw mesh
    vao_->bind();
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<uint32_t>(indices_.size()), GL_UNSIGNED_INT, 0, count);
    vao_->unbind();
}

} // namespace wen