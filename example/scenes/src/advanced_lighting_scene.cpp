#include "advanced_lighting_scene.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

void AdvancedLightingScene::initialize() {
    plane_ = std::make_shared<wen::PlaneGeometry>();

    auto vert = interface_->createShader("scenes/advanced_lighting_scene/vert.glsl",
                                         wen::ShaderStage::eVertex);
    auto frag = interface_->createShader("scenes/advanced_lighting_scene/frag.glsl",
                                         wen::ShaderStage::eFragment);
    shader_program_ = interface_->createShaderProgram();
    shader_program_->attach(vert).attach(frag).link();

    texture1_ = interface_->createTexture2D("wood.png", {
        .gamma = false
    });
    texture2_ = interface_->createTexture2D("wood.png", {
        .gamma = true
    });

    shader_program_->bind();
    shader_program_->setInt("wood", 0);

    camera_uniforms_ = interface_->createUniformBuffer(sizeof(wen::Camera::CameraData));

    skybox_ = std::make_shared<skybox>();
}

void AdvancedLightingScene::update(float ts) {
    camera_->update(ts);
}

void AdvancedLightingScene::render() {
    renderer_->setClearColor(0.0, 0.0f, 0.0f, 1.0f);

    camera_uniforms_->setData(&camera_->data, sizeof(wen::Camera::CameraData));

    renderer_->bindResources(shader_program_);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f));
    shader_program_->setMat4("model", model);
    shader_program_->setVec3("view_pos", camera_->position);
    for (int i = 0; i < 4; i++) {
        shader_program_->setVec3("light_pos[" + std::to_string(i) + "]", glm::vec3(-3.0f+i*2, -0.5f, 0.0f));
        shader_program_->setVec3("light_color[" + std::to_string(i) + "]", glm::vec3(0.25*(i+1)));
    }
    shader_program_->setInt("gamma", gamma);
    renderer_->bindTexture2D(gamma ? texture2_: texture1_, 0);
    renderer_->drawGeometry(plane_);
    renderer_->unbindResources(shader_program_);

    skybox_->draw(renderer_, &(*camera_));
}

void AdvancedLightingScene::imgui() {
    ImGui::Begin("Settings");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Checkbox("gamma", &gamma);
    ImGui::End();
}

void AdvancedLightingScene::destroy() {
    camera_uniforms_.reset();
    plane_.reset();
    shader_program_.reset();
    texture1_.reset();
    texture2_.reset();

    skybox_.reset();
}