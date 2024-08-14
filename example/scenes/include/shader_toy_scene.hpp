#pragma once

#include "scene.hpp"

struct ShaderToyInputUniform {
    alignas(16) glm::vec3 iResolution;
    alignas(4) float iTime;
    alignas(4) float iTimeDelta;
    alignas(4) float iFrameRate;
    alignas(4) float iFrame;
    alignas(16) glm::vec4 iMouse;
    alignas(16) glm::vec4 iDate;
};

class ShaderToyScene : public Scene {
public:
    ShaderToyScene(std::shared_ptr<wen::Interface> interface) : Scene(interface) {}

    void initialize() override;
    void update(float ts) override;
    void render() override;
    void imgui() override;
    void destroy() override;

private:
    float time_ = 0.0f;
    std::unique_ptr<ShaderToyInputUniform> input_;
    std::shared_ptr<wen::UniformBuffer> input_uniforms_;
    std::shared_ptr<wen::ShaderProgram> shader_program_;
    std::shared_ptr<wen::VertexArray> vao_;
};