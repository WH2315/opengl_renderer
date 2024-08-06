#include "wen.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();

    manager->initializeRenderer();

    auto interface = std::make_shared<wen::Interface>("./example/resources");

    auto geometry = std::make_shared<wen::BoxGeometry>();

    auto plane = std::make_shared<wen::PlaneGeometry>();
    auto tours = std::make_shared<wen::TorusGeometry>();

    auto vert = interface->createShader("geometry/geometry.vert", wen::ShaderStage::eVertex);
    auto frag = interface->createShader("geometry/geometry.frag", wen::ShaderStage::eFragment);
    auto program = interface->createShaderProgram(); 
    program->attach(vert)
            .attach(frag)
            .link();
    
    auto light = std::make_shared<wen::SphereGeometry>();
    auto light_vert = interface->createShader("geometry/light.vert", wen::ShaderStage::eVertex);
    auto light_frag = interface->createShader("geometry/light.frag", wen::ShaderStage::eFragment);
    auto light_program = interface->createShaderProgram();
    light_program->attach(light_vert)
            .attach(light_frag)
            .link();

    auto texture1 = interface->createTexture2D("marble.jpg");
    auto texture2 = interface->createTexture2D("metal.png");

    program->bind();
    program->setInt("texture1", 0)
            .setInt("texture2", 1);

    auto renderer = interface->createRenderer();

    auto camera = new wen::Camera();

    auto imgui = new wen::Imgui();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!manager->shouldClose()) {
        manager->pollEvents();

        camera->update(ImGui::GetIO().DeltaTime);

        glDepthFunc(GL_LESS);

        static auto start = std::chrono::high_resolution_clock::now();
        auto current = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float, std::chrono::seconds::period>(current - start).count();

        glm::vec3 light_position;
        light_position.x = 1.0f + sin(time) * 2.0f;
        light_position.y = sin(time / 2.0f) * 1.0f;
        light_position.z = cos(time) * 2.0f;

        renderer->setClearColor(0.0, 0.0f, 0.0f, 1.0f);

        renderer->bindResources(program);
        renderer->bindTexture2D(texture1, 0);
        glm::mat4 model = glm::mat4(1.0f);
        program->setMat4("view", camera->data.view)
                .setMat4("project", camera->data.project);
        program->setVec3("light_position", light_position)
                .setVec3("view_position", camera->data.position);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        program->setMat4("model", model);
        renderer->drawGeometry(geometry);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        program->setMat4("model", model);
        renderer->drawGeometry(geometry);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));
        program->setMat4("model", model);
        renderer->drawGeometry(tours);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.51f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f));
        program->setMat4("model", model);
        renderer->bindTexture2D(texture2, 0);
        renderer->drawGeometry(plane);
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

        imgui->begin();
        ImGui::Begin("Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
        imgui->end();
    }

    delete imgui;
    delete camera;

    renderer.reset();
    texture1.reset();
    texture2.reset();
    light_program.reset();
    light_vert.reset();
    light_frag.reset();
    light.reset();
    program.reset();
    vert.reset();
    frag.reset();
    plane.reset();
    geometry.reset();

    manager->destroyRenderer();

    manager->destroyEngine();

    return 0;
}