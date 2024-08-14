#include "advanced_lighting_scene.hpp"
#include "shader_toy_scene.hpp"

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();
    manager->initializeRenderer();

    auto interface = std::make_shared<wen::Interface>("./example/resources");

    auto scene_manager = new SceneManager(interface);
    scene_manager->addScene<ShaderToyScene>("easy shader toy");
    scene_manager->addScene<AdvancedLightingScene>("advanced lighting");
    while (!manager->shouldClose()) {
        manager->pollEvents();
        scene_manager->update(ImGui::GetIO().DeltaTime);
        scene_manager->render();
    }
    delete scene_manager;

    manager->destroyRenderer();
    manager->destroyEngine();

    delete manager;

    return 0;
}