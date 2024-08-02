#include "wen.hpp"
#include <glm/gtc/type_ptr.hpp>

float delta_time = 0.0f;
float last_frame = 0.0f;

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();

    manager->initializeRenderer();

    auto interface = std::make_shared<wen::Interface>("./sandbox/resources");

    auto vert = interface->createShader("vert.glsl", wen::ShaderStage::eVertex);
    auto frag = interface->createShader("frag.glsl", wen::ShaderStage::eFragment);
    auto program = interface->createShaderProgram();
    program->attach(vert)
            .attach(frag)
            .link();
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    auto VAO = interface->createVertexArray();
    auto VBO = interface->createVertexBuffer(sizeof(vertices));
    VBO->setData(vertices);

    VBO->setVertexLayout({
        {"positon", wen::VertexType::eFloat3},
        {"tex_coords", wen::VertexType::eFloat2}
    });
    VAO->attachVertexBuffer(VBO);

    auto texture1 = interface->createTexture2D(
        "container.jpg",
        {
            .min_filter = wen::TextureFilter::eLinear,
            .mag_filter = wen::TextureFilter::eLinear,
            .wrap_s = wen::TextureWrap::eClampToEdge,
            .wrap_t = wen::TextureWrap::eClampToEdge,
        } 
    );
    auto texture2 = interface->createTexture2D("awesomeface.png");

    program->bind();
    program->setInt("texture1", 0)
            .setInt("texture2", 1);

    auto renderer = interface->createRenderer();

    auto camera = new wen::Camera();
    
    auto imgui = new wen::Imgui();

    while (!manager->shouldClose()) {
        manager->pollEvents();

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        camera->update(delta_time);

        renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer->bindResources(program, VAO);
        renderer->bindTexture2D(texture1, 0);
        renderer->bindTexture2D(texture2, 1);

        glm::mat4 model = glm::mat4(1.0f);
        float angle = current_frame * 30.0f;;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        program->setMat4("model", model);
        program->setMat4("view", camera->data.view);
        program->setMat4("project", camera->data.project);

        renderer->draw(36);

        renderer->unbindResources(program, VAO);

        imgui->begin();
        ImGui::ShowDemoWindow();
        imgui->end();
    }

    delete imgui;
    delete camera;

    renderer.reset();
    texture1.reset();
    texture2.reset();
    VAO.reset();
    VBO.reset();
    program.reset();
    vert.reset();
    frag.reset();

    interface.reset();

    manager->destroyRenderer();

    manager->destroyEngine();

    manager = nullptr;
    delete manager;

    return 0;
}