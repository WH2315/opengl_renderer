#include "wen.hpp"

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
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    auto VAO = interface->createVertexArray();
    auto VBO = interface->createVertexBuffer(vertices, sizeof(vertices));

    VBO->setVertexLayout({
        {"positon", wen::VertexType::eFloat3, false}
    });
    VAO->attachVertexBuffer(VBO);

    auto renderer = interface->createRenderer();

    while (!manager->shouldClose()) {
        manager->pollEvents();
        renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer->bindShaderProgram(program);
        renderer->bindVertexArray(VAO);
        renderer->draw(3);
    }

    renderer.reset();
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