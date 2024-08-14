#include "scene.hpp"

void imgui_dockspace_begin() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus;
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

void imgui_dockspace_end() {
    ImGui::End();
}

SceneManager::SceneManager(std::shared_ptr<wen::Interface> interface) : interface_(interface) {
    wen::FramebufferAttachment framebuffer_attachment = {
        .width = 800,
        .height = 800,
        .attachment = {
            wen::AttachmentType::eColor,
            wen::AttachmentType::eDepthStencil
        }
    };
    framebuffer_ = interface_->createFramebuffer(framebuffer_attachment);
}

void SceneManager::update(float ts) {
    if (current_scene_.get() == nullptr) {
        return;
    }

    if (auto a = framebuffer_->getFramebufferAttachment();
        ViewportSize.x > 0 && ViewportSize.y > 0 && (a.width != ViewportSize.x || a.height != ViewportSize.y)) {
        framebuffer_->resize(ViewportSize.x, ViewportSize.y);
        current_scene_->scene_size = ViewportSize;
        current_scene_->camera_->resize(ViewportSize.x, ViewportSize.y);
    }
    current_scene_->update(ts);
}

void SceneManager::render() {
    if (current_scene_.get() == nullptr) {
        return;
    }

    framebuffer_->bind();
    current_scene_->render();
    framebuffer_->unbind();

    current_scene_->imgui_->begin();
    imgui_dockspace_begin();
    current_scene_->imgui();
    ImGui::Begin("Scene Manager");
    for (auto& [name, load_scene] : scenes_) {
        if (ImGui::Button(name.c_str())) {
            changed_scene_ = load_scene;
        }
    }
    ImGui::End();
    ImGui::Begin("Scene");
    ViewportSize = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
    ImGui::Image(reinterpret_cast<void*>(framebuffer_->getTexture()),
                 ImVec2{ViewportSize.x, ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    imgui_dockspace_end();
    current_scene_->imgui_->end();
    
    if (changed_scene_.has_value()) {
        changed_scene_.value()();
        changed_scene_.reset();
    }
}

SceneManager::~SceneManager() {
    if (current_scene_.get() != nullptr) {
        current_scene_->destroy();
        current_scene_.reset();
    }

    interface_.reset();
    framebuffer_.reset();
}