#include "interface.hpp"

namespace wen {

Interface::Interface(const std::string& path) : path_(path) {
    shader_dir_ = path_ + "/shaders";
    texture_dir_ = path_ + "/textures";
    model_dir_ = path_ + "/models";
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

std::shared_ptr<Texture2D> Interface::createTexture2D(const std::string& filename, const TextureInfos& infos) {
    return std::make_shared<Texture2D>(texture_dir_ + "/" + filename, infos);
}

std::shared_ptr<Model> Interface::loadModel(const std::string& filename) {
    return std::make_shared<Model>(model_dir_ + "/" + filename);
}

std::shared_ptr<Framebuffer> Interface::createFramebuffer(const FramebufferAttachment& framebuffer_attachment) {
    return std::make_shared<Framebuffer>(framebuffer_attachment);
}

std::shared_ptr<UniformBuffer> Interface::createUniformBuffer(uint32_t size, uint32_t binding) {
    return std::make_shared<UniformBuffer>(size, binding);
}

} // namespace wen