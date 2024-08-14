#include "shader_toy_scene.hpp"

void ShaderToyScene::initialize() {
    auto vert = interface_->createShader("scenes/shader_toy/shader.vert",
                                         wen::ShaderStage::eVertex);
    auto frag = interface_->createShader("scenes/shader_toy/shader.frag",
                                         wen::ShaderStage::eFragment);
    shader_program_ = interface_->createShaderProgram();
    shader_program_->attach(vert).attach(frag).link();

    std::vector<glm::vec2> pos = {
        { 1.0f,  1.0f},
        {-1.0f,  1.0f},
        { 1.0f, -1.0f},
        {-1.0f, -1.0f},
    };
    std::vector<uint16_t> indices = {
        0, 1, 2, 1, 2, 3
    };
    vao_ = interface_->createVertexArray();
    auto vbo = interface_->createVertexBuffer(sizeof(pos));
    auto ibo = interface_->createIndexBuffer(wen::IndexType::eUInt16, 6);
    vbo->setData(pos.data());
    ibo->setData(indices.data());
    vbo->setVertexLayout({
        {"pos", wen::VertexType::eFloat2},
    });
    vao_->attachVertexBuffer(vbo);
    vao_->attachIndexBuffer(ibo);

    input_ = std::make_unique<ShaderToyInputUniform>();
    input_uniforms_ = interface_->createUniformBuffer(sizeof(ShaderToyInputUniform));
}

void ShaderToyScene::update(float ts) {
    auto w = scene_size.x, h = scene_size.y;
    shader_program_->bind();
    shader_program_->setFloat("scene_width", w)
                    .setFloat("scene_height", h);

    time_ += ts;
    input_->iResolution = glm::vec3(w, h, 1.0f);
    input_->iTime = time_;
    input_->iTimeDelta = ts;
}

void ShaderToyScene::render() {
    renderer_->setClearColor(0.0, 0.0f, 0.0f, 1.0f);

    input_uniforms_->setData(input_.get(), sizeof(ShaderToyInputUniform)); 

    renderer_->bindResources(shader_program_, vao_);
    renderer_->drawIndexed(wen::IndexType::eUInt16, 6);
    renderer_->unbindResources(shader_program_, vao_);
}

void ShaderToyScene::imgui() {
    ImGui::Begin("Settings");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void ShaderToyScene::destroy() {
    input_.reset();
    input_uniforms_.reset();
    shader_program_.reset();
    vao_.reset();
}