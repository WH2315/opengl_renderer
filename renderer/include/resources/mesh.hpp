#pragma once

#include "resources/shader_program.hpp"
#include "resources/vertex_array.hpp"

namespace wen {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct ModelTexture {
    uint32_t texture;
    std::string type;
    std::string path;
};

class Mesh {
    friend class Model;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<ModelTexture> textures);
    
    void draw(const std::shared_ptr<ShaderProgram>& program, uint32_t count = 1);

    auto& vao() const { return vao_; }

private:
    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    std::vector<ModelTexture> textures_;
    std::shared_ptr<VertexArray> vao_;
};

} // namespace wen