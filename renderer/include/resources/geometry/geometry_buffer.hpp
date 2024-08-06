#pragma once

#include "resources/vertex_array.hpp"
#include <glm/glm.hpp>

namespace wen {

class GeometryBuffer {
protected:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::shared_ptr<VertexArray> vao;

    void upload() {
        vao = std::make_shared<VertexArray>();
        auto vbo = std::make_shared<VertexBuffer>(vertices.size() * sizeof(Vertex));
        auto ibo = std::make_shared<IndexBuffer>(IndexType::eUInt32, indices.size());

        vbo->setVertexLayout({
            {"positon", VertexType::eFloat3},
            {"normal", VertexType::eFloat3},
            {"tex_coords", VertexType::eFloat2}
        });

        vbo->setData(vertices.data());
        ibo->setData(indices.data());

        vao->attachVertexBuffer(vbo);
        vao->attachIndexBuffer(ibo);
    }

public:
    void draw() {
        vao->bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        vao->unbind();
    }
};

} // namespace wen