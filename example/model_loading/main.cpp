#include "wen.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();

    manager->initializeRenderer();

    auto interface = std::make_shared<wen::Interface>("./example/resources");

    auto vert = interface->createShader("model_loading/model.vert", wen::ShaderStage::eVertex);
    auto frag = interface->createShader("model_loading/model.frag", wen::ShaderStage::eFragment);
    auto light_vert = interface->createShader("model_loading/light.vert", wen::ShaderStage::eVertex);
    auto light_frag = interface->createShader("model_loading/light.frag", wen::ShaderStage::eFragment);

    auto program = interface->createShaderProgram();
    program->attach(vert)
            .attach(frag)
            .link();
    auto light_program = interface->createShaderProgram();
    light_program->attach(light_vert)
            .attach(light_frag)
            .link();
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    auto light_VAO = interface->createVertexArray();
    auto light_VBO = interface->createVertexBuffer(sizeof(vertices));
    light_VBO->setData(vertices);
    light_VBO->setVertexLayout({
        {"position", wen::VertexType::eFloat3},
    });
    light_VAO->attachVertexBuffer(light_VBO);

    auto model = interface->loadModel("nanosuit/nanosuit.obj");

    auto renderer = interface->createRenderer();

    auto camera = new wen::Camera();

    auto imgui = new wen::Imgui;

    while (!manager->shouldClose()) {
        manager->pollEvents();

        camera->update(ImGui::GetIO().DeltaTime);

        static auto start = std::chrono::high_resolution_clock::now();
        auto current = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float, std::chrono::seconds::period>(current - start).count();

        glm::vec3 light_position = glm::rotateY(glm::vec3(2.0f, 3.0f, 2.0f), time);

        renderer->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        renderer->bindResources(program);
        glm::mat4 _model = glm::mat4(1.0f);
        _model = glm::translate(_model, glm::vec3(0.0f, 0.0f, 0.0f));
        _model = glm::scale(_model, glm::vec3(0.3f));
        program->setMat4("model", _model)
                .setMat4("view", camera->data.view)
                .setMat4("project", camera->data.project);
        program->setVec3("view_position", camera->data.position);
        program->setVec3("light.position", light_position)
                .setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f))
                .setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f))
                .setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f))
                .setFloat("light.constant", 1.0f)
                .setFloat("light.linear", 0.09f)
                .setFloat("light.quadratic", 0.032f);
        program->setVec3("dir_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f))
                .setVec3("dir_light.ambient", glm::vec3(0.05f, 0.05f, 0.05f))
                .setVec3("dir_light.diffuse", glm::vec3(0.4f, 0.4f, 0.4f))
                .setVec3("dir_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        renderer->drawModel(model, program);
        renderer->unbindResources(program);

        renderer->bindResources(light_program, light_VAO);
        _model = glm::mat4(1.0f);
        _model = glm::translate(_model, glm::vec3(light_position));
        _model = glm::scale(_model, glm::vec3(0.3f));
        light_program->setMat4("model", _model)
                      .setMat4("view", camera->data.view)
                      .setMat4("project", camera->data.project);
        renderer->draw(36);
        renderer->unbindResources(light_program, light_VAO);

        imgui->begin();
        ImGui::Begin("Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Begin("Scene");
        ImGui::Text("camera position: (%.2f, %.2f, %.2f)", camera->data.position.x, camera->data.position.y, camera->data.position.z);
        ImGui::Text("light position: (%.2f, %.2f, %.2f)", light_position.x, light_position.y, light_position.z);
        ImGui::End();
        imgui->end();
    }

    delete imgui;
    delete camera;

    renderer.reset();
    model.reset();
    light_VAO.reset();
    light_VBO.reset();
    program.reset();
    light_program.reset();
    vert.reset();
    frag.reset();
    light_vert.reset();
    light_frag.reset();

    manager->destroyRenderer();

    manager->destroyEngine();

    return 0;
}