#include "wen.hpp"
#include <glm/gtc/type_ptr.hpp>

float delta_time = 0.0f;
float last_frame = 0.0f;
glm::vec3 light_position(1.2f, 1.0f, 2.0f);
glm::vec3 light_color(1.0f, 1.0f, 1.0f);

glm::vec3 object_color(1.0f, 0.5f, 0.31f);

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();

    manager->initializeRenderer();

    auto interface = std::make_shared<wen::Interface>("./example/resources");

    auto cube_vert = interface->createShader("lighting/cube.vert", wen::ShaderStage::eVertex);
    auto cube_frag = interface->createShader("lighting/cube.frag", wen::ShaderStage::eFragment);
    auto light_vert = interface->createShader("lighting/light.vert", wen::ShaderStage::eVertex);
    auto light_frag = interface->createShader("lighting/light.frag", wen::ShaderStage::eFragment);
    auto cube_program = interface->createShaderProgram(); 
    auto light_program = interface->createShaderProgram(); 
    cube_program->attach(cube_vert)
                .attach(cube_frag)
                .link();
    light_program->attach(light_vert)
                .attach(light_frag)
                .link();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    auto cube_VAO = interface->createVertexArray(); 
    auto light_VAO = interface->createVertexArray();
    auto VBO = interface->createVertexBuffer(sizeof(vertices));
    VBO->setData(vertices);

    VBO->setVertexLayout({
        {"position", wen::VertexType::eFloat3},
        {"normal", wen::VertexType::eFloat3}
    });
    cube_VAO->attachVertexBuffer(VBO);
    light_VAO->attachVertexBuffer(VBO);

    auto renderer = interface->createRenderer();

    auto camera = new wen::Camera();
    camera->data.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera->upload();

    auto imgui = new wen::Imgui();

    while (!manager->shouldClose()) {
        manager->pollEvents();

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        camera->update(delta_time);

        renderer->setClearColor(0.0, 0.0f, 0.0f, 1.0f);

        light_position.x = 1.0f + sin(current_frame) * 2.0f;
        light_position.y = sin(current_frame / 2.0f) * 1.0f;

        renderer->bindShaderProgram(cube_program);
        renderer->bindVertexArray(cube_VAO);
        glm::mat4 model = glm::mat4(1.0f);
        float angle = current_frame * 30.0f;;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        cube_program->setMat4("model", model);
        cube_program->setMat4("view", camera->data.view);
        cube_program->setMat4("project", camera->data.project);

        cube_program->setVec3("light_position", light_position);
        cube_program->setVec3("view_position", camera->data.position);
        cube_program->setVec3("light_color",  light_color);
        cube_program->setVec3("object_color", object_color);
        renderer->draw(36);

        renderer->bindShaderProgram(light_program);
        renderer->bindVertexArray(light_VAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_position);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        light_program->setMat4("model", model);
        light_program->setMat4("view", camera->data.view);
        light_program->setMat4("project", camera->data.project);
        light_program->setVec3("light_color", light_color);
        renderer->draw(36);

        imgui->begin();
        ImGui::Begin("Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Begin("Scene");
        ImGui::ColorEdit3("light color", glm::value_ptr(light_color));
        ImGui::ColorEdit3("object color", glm::value_ptr(object_color));
        ImGui::End();
        imgui->end();
    }

    delete imgui;
    delete camera;

    renderer.reset();
    cube_VAO.reset();
    light_VAO.reset();
    VBO.reset();
    cube_program.reset();
    light_program.reset();
    cube_vert.reset();
    cube_frag.reset();
    light_vert.reset();
    light_frag.reset();

    manager->destroyRenderer();

    manager->destroyEngine();

    return 0;
}