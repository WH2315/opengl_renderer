#pragma once

#include "resources/shader.hpp"
#include "resources/shader_program.hpp"
#include "resources/vertex_array.hpp"
#include "resources/vertex_buffer.hpp"
#include "resources/index_buffer.hpp"
#include "renderer.hpp"

namespace wen {

class Interface final {
public:
    Interface(const std::string& path);

    std::shared_ptr<Shader> createShader(const std::string& filename, ShaderStage stage);
    std::shared_ptr<ShaderProgram> createShaderProgram();
    std::shared_ptr<VertexArray> createVertexArray();
    std::shared_ptr<VertexBuffer> createVertexBuffer(uint32_t size);
    std::shared_ptr<IndexBuffer> createIndexBuffer(IndexType type, uint32_t count);
    std::shared_ptr<Renderer> createRenderer();

private:
    std::string path_;
    std::string shader_dir_;
};

} // namespace wen