#include "wen.hpp"
#include <glm/gtc/type_ptr.hpp>

float delta_time = 0.0f;
float last_frame = 0.0f;
glm::vec3 light_position(1.2f, 1.0f, 2.0f);

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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    auto cube_VAO = interface->createVertexArray(); 
    auto light_VAO = interface->createVertexArray();
    auto VBO = interface->createVertexBuffer(sizeof(vertices));
    VBO->setData(vertices);

    VBO->setVertexLayout({
        {"position", wen::VertexType::eFloat3},
        {"normal", wen::VertexType::eFloat3},
        {"tex_coords", wen::VertexType::eFloat2}
    });
    cube_VAO->attachVertexBuffer(VBO);
    light_VAO->attachVertexBuffer(VBO);

    auto container = interface->createTexture2D("lighting/container2.png");
    auto container_specular = interface->createTexture2D("lighting/container2_specular.png");

    cube_program->bind();
    cube_program->setInt("material.diffuse", 0)
                 .setInt("material.specular", 1)
                 .setFloat("material.shininess", 32.0f);
    cube_program->setVec3("dir_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f))
                 .setVec3("dir_light.ambient", glm::vec3(0.05f, 0.05f, 0.05f))
                 .setVec3("dir_light.diffuse", glm::vec3(0.4f, 0.4f, 0.4f))
                 .setVec3("dir_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    auto renderer = interface->createRenderer();

    auto camera = new wen::Camera();
    camera->data.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera->upload();

    auto imgui = new wen::Imgui();

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while (!manager->shouldClose()) {
        manager->pollEvents();

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        camera->update(delta_time);

        renderer->setClearColor(0.0, 0.0f, 0.0f, 1.0f);

        light_position.x = 1.0f + sin(current_frame) * 2.0f;
        light_position.y = sin(current_frame / 2.0f) * 1.0f;
        light_position.z = cos(current_frame) * 2.0f;

        renderer->bindResources(cube_program, cube_VAO);
        renderer->bindTexture2D(container, 0);
        renderer->bindTexture2D(container_specular, 1);
        cube_program->setMat4("view", camera->data.view);
        cube_program->setMat4("project", camera->data.project);
        cube_program->setVec3("point_light.position", light_position)
                     .setVec3("point_light.ambient", glm::vec3(0.2f, 0.2f, 0.2f))
                     .setVec3("point_light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f))
                     .setVec3("point_light.specular", glm::vec3(1.0f, 1.0f, 1.0f))
                     .setFloat("point_light.constant", 1.0f)
                     .setFloat("point_light.linear", 0.09f)
                     .setFloat("point_light.quadratic", 0.032f);
        cube_program->setVec3("spotlight.position", camera->data.position)
                     .setVec3("spotlight.direction", camera->direction)
                     .setVec3("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f))
                     .setVec3("spotlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f))
                     .setVec3("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f))
                     .setFloat("spotlight.constant", 1.0f)
                     .setFloat("spotlight.linear", 0.09f)
                     .setFloat("spotlight.quadratic", 0.032f)
                     .setFloat("spotlight.cut_off", glm::cos(glm::radians(12.5f)))
                     .setFloat("spotlight.outer_cut_off", glm::cos(glm::radians(15.0f)));
        cube_program->setVec3("view_position", camera->data.position);
        glm::mat4 model = glm::mat4(1.0f);
        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle) * (i % 3 == 0 ? 1.0f : -1.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            cube_program->setMat4("model", model);
            renderer->draw(36);
        }
        renderer->unbindResources(cube_program, cube_VAO);

        renderer->bindResources(light_program, light_VAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_position);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        light_program->setMat4("model", model);
        light_program->setMat4("view", camera->data.view);
        light_program->setMat4("project", camera->data.project);
        renderer->draw(36);
        renderer->unbindResources(light_program, light_VAO);

        imgui->begin();
        ImGui::Begin("Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
        imgui->end();
    }

    delete imgui;
    delete camera;

    renderer.reset();
    container.reset();
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