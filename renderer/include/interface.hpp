#pragma once

#include "resources/shader.hpp"
#include "resources/shader_program.hpp"
#include "resources/vertex_array.hpp"
#include "resources/vertex_buffer.hpp"
#include "resources/index_buffer.hpp"
#include "renderer.hpp"
#include "resources/texture_2d.hpp"
#include "resources/model.hpp"
#include "resources/framebuffer.hpp"
#include "resources/uniform_buffer.hpp"

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
    std::shared_ptr<Texture2D> createTexture2D(const std::string& filename, const TextureInfos& infos = {});
    std::shared_ptr<Model> loadModel(const std::string& filename);
    std::shared_ptr<Framebuffer> createFramebuffer(const FramebufferAttachment& framebuffer_attachment);
    std::shared_ptr<UniformBuffer> createUniformBuffer(uint32_t size, uint32_t binding = 0);

private:
    std::string path_;
    std::string shader_dir_;
    std::string texture_dir_;
    std::string model_dir_;
};

} // namespace wen