#include "wen.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

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
    
    std::vector<Vertex> vertices = {
        {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},   // 右上角
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // 右下角
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // 左下角
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}   // 左上角
    };

    std::vector<uint16_t> indices = {
        0, 1, 3, 1, 2, 3
    };

    auto VAO = interface->createVertexArray();
    auto VBO = interface->createVertexBuffer(sizeof(Vertex) * vertices.size());
    VBO->setData(vertices.data());
    auto IBO = interface->createIndexBuffer(wen::IndexType::eUInt16, indices.size());
    IBO->setData(indices.data());

    VBO->setVertexLayout({
        {"positon", wen::VertexType::eFloat3},
        {"color", wen::VertexType::eFloat3}
    });
    VAO->attachVertexBuffer(VBO);
    VAO->attachIndexBuffer(IBO);

    float offset = 0.3f;
    program->setFloat("xOffset", offset);

    auto renderer = interface->createRenderer();

    while (!manager->shouldClose()) {
        manager->pollEvents();
        renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer->bindShaderProgram(program);
        renderer->bindVertexArray(VAO);
        // renderer->draw(3);
        renderer->drawIndexed(wen::IndexType::eUInt16, indices.size());
    }

    renderer.reset();
    VAO.reset();
    VBO.reset();
    IBO.reset();
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