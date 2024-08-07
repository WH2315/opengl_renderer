#include "wen.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

bool is_draw_outline = false;
glm::vec2 ViewportSize;

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

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();

    manager->initializeRenderer();

    auto interface = std::make_shared<wen::Interface>("./example/resources");

    auto cube = std::make_shared<wen::BoxGeometry>();

    auto plane = std::make_shared<wen::PlaneGeometry>();
    auto tours = std::make_shared<wen::TorusGeometry>();

    auto vert = interface->createShader("geometry/geometry.vert", wen::ShaderStage::eVertex);
    auto frag = interface->createShader("geometry/geometry.frag", wen::ShaderStage::eFragment);
    auto program = interface->createShaderProgram(); 
    program->attach(vert)
            .attach(frag)
            .link();
    
    auto stencil_color_frag = interface->createShader("geometry/stencil_single_color.frag", wen::ShaderStage::eFragment);
    auto stencil_program = interface->createShaderProgram();
    stencil_program->attach(vert)
            .attach(stencil_color_frag)
            .link();
    
    auto light = std::make_shared<wen::SphereGeometry>();
    auto light_frag = interface->createShader("geometry/light.frag", wen::ShaderStage::eFragment);
    auto light_program = interface->createShaderProgram();
    light_program->attach(vert)
            .attach(light_frag)
            .link();

    auto texture1 = interface->createTexture2D("marble.jpg");
    auto texture2 = interface->createTexture2D("metal.png");
    auto window = interface->createTexture2D("window.png", {
        .min_filter = wen::TextureFilter::eLinearMipmapLinear,
        .mag_filter = wen::TextureFilter::eLinear,
        .wrap_s = wen::TextureWrap::eClampToEdge,
        .wrap_t = wen::TextureWrap::eClampToEdge,
    });

    program->bind();
    program->setInt("texture1", 0);

    std::vector<glm::vec3> windows;
    windows.push_back(glm::vec3(-1.5f+0.5f,  0.0f, -0.48f));
    windows.push_back(glm::vec3( 1.5f+0.5f,  0.0f,  0.51f));
    windows.push_back(glm::vec3( 0.0f+0.5f,  0.0f,  0.7f));
    windows.push_back(glm::vec3(-0.3f+0.5f,  0.0f, -2.3f));
    windows.push_back(glm::vec3( 0.5f+0.5f,  0.0f, -0.6f));

    auto renderer = interface->createRenderer();

    auto camera = new wen::Camera();

    auto imgui = new wen::Imgui();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    wen::FramebufferAttachment framebuffer_attachment = {
        .width = 600,
        .height = 600,
        .attachment = {
            wen::AttachmentType::eColor,
            wen::AttachmentType::eDepthStencil
        }
    };
    auto framebuffer = interface->createFramebuffer(framebuffer_attachment);
    
    while (!manager->shouldClose()) {
        manager->pollEvents();

        if (auto a = framebuffer->getFramebufferAttachment();
            ViewportSize.x > 0 && ViewportSize.y > 0 && (a.width != ViewportSize.x || a.height != ViewportSize.y)) {
            framebuffer->resize(ViewportSize.x, ViewportSize.y);
            camera->resize(ViewportSize.x, ViewportSize.y);
        }

        camera->update(ImGui::GetIO().DeltaTime);

        std::map<float, glm::vec3> sorted;
        for (uint32_t i = 0; i < windows.size(); i++) {
            float distance = glm::length(camera->data.position - windows[i]);
            sorted[distance] = windows[i];
        }

        static auto start = std::chrono::high_resolution_clock::now();
        auto current = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float, std::chrono::seconds::period>(current - start).count();

        glm::vec3 light_position;
        light_position.x = 1.0f + sin(time) * 2.0f;
        light_position.y = sin(time / 2.0f) * 1.0f;
        light_position.z = cos(time) * 2.0f;

        framebuffer->bind();

        renderer->setClearColor(0.0, 0.0f, 0.0f, 1.0f);

        glStencilMask(0x00);
        renderer->bindResources(program);
        glm::mat4 model = glm::mat4(1.0f);
        program->setMat4("view", camera->data.view)
                .setMat4("project", camera->data.project);
        program->setVec3("light_position", light_position)
                .setVec3("view_position", camera->data.position);
        // draw floor
        renderer->bindTexture2D(texture2, 0);
        model = glm::translate(model, glm::vec3(0.0f, -0.51f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f));
        program->setMat4("model", model);
        renderer->drawGeometry(plane);
        // draw cube
        // 对箱子中所有绘制的片段, 将模板值更新为1
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
        glStencilMask(0xFF); // 启用模板缓冲写入
        renderer->bindTexture2D(texture1, 0);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        program->setMat4("model", model);
        renderer->drawGeometry(cube);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        program->setMat4("model", model);
        renderer->drawGeometry(cube);
        renderer->unbindResources(program);
        // 现在模板缓冲在箱子被绘制的地方都更新为1了

        if (is_draw_outline) {
            // 只有模板缓冲中值不等于1的片段才会被绘制
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); // 禁止模板缓冲的写入
            glDisable(GL_DEPTH_TEST);
            renderer->bindResources(stencil_program);
            stencil_program->setMat4("view", camera->data.view)
                            .setMat4("project", camera->data.project);
            float scale = 1.1f;
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            model = glm::scale(model, glm::vec3(scale));
            stencil_program->setMat4("model", model);
            renderer->drawGeometry(cube);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(scale));
            stencil_program->setMat4("model", model);
            renderer->drawGeometry(cube);
            renderer->unbindResources(stencil_program);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);
        }

        // draw tours
        renderer->bindResources(program);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));
        program->setMat4("model", model);
        renderer->drawGeometry(tours);
        renderer->unbindResources(program);

        renderer->bindResources(light_program);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(light_position));
        model = glm::scale(model, glm::vec3(0.2f));
        light_program->setMat4("model", model)
                      .setMat4("view", camera->data.view)
                      .setMat4("project", camera->data.project);
        renderer->drawGeometry(light);
        renderer->unbindResources(light_program);

        // draw windows
        renderer->bindResources(program);
        renderer->bindTexture2D(window, 0);
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            program->setMat4("model", model);
            renderer->drawGeometry(plane);
        }
        renderer->unbindResources(program);

        framebuffer->unbind();

        imgui->begin();
        imgui_dockspace_begin(); 
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::Begin("Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Checkbox("Draw Outline", &is_draw_outline);
        ImGui::End();
        ImGui::Begin("Scene");
        ViewportSize = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
        ImGui::Image(reinterpret_cast<void*>(framebuffer->getTexture()),
                     ImVec2{ViewportSize.x, ViewportSize.y}, ImVec2{0, 1},
                     ImVec2{1, 0});
        ImGui::End();
        imgui_dockspace_end();
        imgui->end();
    }

    delete imgui;
    delete camera;

    framebuffer.reset();
    renderer.reset();
    texture1.reset();
    texture2.reset();
    window.reset();
    light_program.reset();
    light_frag.reset();
    light.reset();
    stencil_program.reset();
    stencil_color_frag.reset();
    program.reset();
    vert.reset();
    frag.reset();
    plane.reset();
    cube.reset();

    manager->destroyRenderer();

    manager->destroyEngine();

    return 0;
}