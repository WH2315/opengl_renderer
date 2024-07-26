#pragma once

#include <glm/glm.hpp>

namespace wen {

class Camera final {
public:
    Camera();

    void update(float ts);
    void upload();

    struct CameraData {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::mat4 view;
        glm::mat4 project;
    } data;

    glm::vec3 direction;
    bool cursor_locked = false;
};

} // namespace wen