#pragma once

#include "wen.hpp"
#include <optional>

class Scene {
    friend class SceneManager;

public:
    Scene(std::shared_ptr<wen::Interface> interface) : interface_(interface) {
        renderer_ = std::make_shared<wen::Renderer>();
        camera_ = std::make_shared<wen::Camera>();
        imgui_ = std::make_shared<wen::Imgui>();
    }

    virtual ~Scene() = default;

    virtual void initialize() = 0;
    virtual void update(float ts) = 0;
    virtual void render() = 0;
    virtual void imgui() = 0;
    virtual void destroy() = 0;

protected:
    glm::vec2 scene_size = {0, 0};
    std::shared_ptr<wen::Interface> interface_;
    std::shared_ptr<wen::Renderer> renderer_;
    std::shared_ptr<wen::Camera> camera_;
    std::shared_ptr<wen::Imgui> imgui_;
};

class SceneManager {
public:
    SceneManager(std::shared_ptr<wen::Interface> interface);
    ~SceneManager();

    template <class Scene>
    void addScene(const std::string& name) {
        scenes_.insert(std::make_pair(name, [this]() {
            if (current_scene_.get() != nullptr) {
                current_scene_->destroy();
                current_scene_.reset();
            }
            current_scene_ = std::make_unique<Scene>(interface_);
            current_scene_->initialize();
        }));
        if (current_scene_.get() == nullptr) {
            scenes_.begin()->second();
        }
    }

    void update(float ts);
    void render();

private:
    glm::vec2 ViewportSize;

    std::shared_ptr<wen::Interface> interface_;
    std::unique_ptr<Scene> current_scene_;

    std::shared_ptr<wen::Framebuffer> framebuffer_;

    std::map<std::string, std::function<void()>> scenes_;
    std::optional<std::function<void()>> changed_scene_;
};