#include "interface.hpp"

namespace wen {

Interface::Interface(const std::string& path) : path_(path) {
    shader_dir_ = path_ + "/shaders";
}

std::shared_ptr<Shader> Interface::createShader(const std::string& filename, ShaderStage stage) {
    return std::make_shared<Shader>(shader_dir_ + "/" + filename, stage);
}

std::shared_ptr<ShaderProgram> Interface::createShaderProgram() {
    return std::make_shared<ShaderProgram>();
}

std::shared_ptr<VertexArray> Interface::createVertexArray() {
    return std::make_shared<VertexArray>();
}

std::shared_ptr<VertexBuffer> Interface::createVertexBuffer(uint32_t size) {
    return std::make_shared<VertexBuffer>(size);
}

std::shared_ptr<IndexBuffer> Interface::createIndexBuffer(IndexType type, uint32_t count) {
    return std::make_shared<IndexBuffer>(type, count);
}

std::shared_ptr<Renderer> Interface::createRenderer() {
    return std::make_shared<Renderer>();
}

} // namespace wen