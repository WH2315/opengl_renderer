#pragma once

#include "scene.hpp"
#include "../../skybox.hpp"

class AdvancedLightingScene : public Scene {
public:
    AdvancedLightingScene(std::shared_ptr<wen::Interface> interface) : Scene(interface) {}

    void initialize() override;
    void update(float ts) override;
    void render() override;
    void imgui() override;
    void destroy() override;

private:
    bool gamma = false;

    std::shared_ptr<wen::UniformBuffer> camera_uniforms_;
    std::shared_ptr<wen::PlaneGeometry> plane_;
    std::shared_ptr<wen::ShaderProgram> shader_program_;
    std::shared_ptr<wen::Texture2D> texture1_;
    std::shared_ptr<wen::Texture2D> texture2_;

    std::shared_ptr<skybox> skybox_;
};