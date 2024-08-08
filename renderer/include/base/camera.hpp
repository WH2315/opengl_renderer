#pragma once

#include <glm/glm.hpp>

namespace wen {

class Camera final {
public:
    Camera();

    void update(float ts);
    void resize(uint32_t width, uint32_t height);
    void upload();

    struct CameraData {
        glm::mat4 view;
        glm::mat4 project;
    } data;

    glm::vec3 position;
    glm::vec3 direction;
    bool cursor_locked = false;

    uint32_t width, height;
};

} // namespace wen