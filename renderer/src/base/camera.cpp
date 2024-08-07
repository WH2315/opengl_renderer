#include "base/camera.hpp"
#include "core/window.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace wen {

Camera::Camera() {
    data.position = glm::vec3(0.0f, 0.0f, 3.0f);
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    width = g_window->getData().width, height = g_window->getData().height;
    upload();
}

void Camera::update(float ts) {
    GLFWwindow* window = static_cast<GLFWwindow*>(g_window->getWindow());

    static glm::dvec2 last = {0.0, 0.0};
    static bool first = true;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    glm::dvec2 now = {x, y};
    if (first) {
        first = false;
        last = now;
        return;
    }

    static bool space_down = false;
    int space_state = glfwGetKey(window, GLFW_KEY_SPACE);
    if (!space_down && space_state == GLFW_PRESS) {
        space_down = true;
    } else if (space_down && space_state == GLFW_RELEASE) {
        space_down = false;
        cursor_locked = !cursor_locked;
        glfwSetInputMode(window, GLFW_CURSOR, cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (!cursor_locked) {
        last = now;
        return;
    }

    constexpr glm::vec3 up_direction(0.0f, 1.0f, 0.0f);
    glm::vec3 right_direction = glm::cross(direction, up_direction);
    float speed = 3.0f;
    if (glfwGetKey(window, GLFW_KEY_W)) {
        data.position += direction * speed * ts;
    } else if (glfwGetKey(window, GLFW_KEY_S)) {
        data.position -= direction * speed * ts;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        data.position -= right_direction * speed * ts;
    } else if (glfwGetKey(window, GLFW_KEY_D)) {
        data.position += right_direction * speed * ts;
    }
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        data.position += up_direction * speed * ts;
    } else if (glfwGetKey(window, GLFW_KEY_E)) {
        data.position -= up_direction * speed * ts;
    }

    glm::dvec2 delta = (now - last) * 0.002;
    last = now;
    if (delta.x != 0.0 || delta.y != 0.0) {
        float pitch = delta.y * 0.3f;
        float yaw = delta.x * 0.3f;
        glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitch, right_direction), glm::angleAxis(-yaw, up_direction)));
        direction = glm::rotate(q, direction);
    }

    upload();
}

void Camera::resize(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
    upload();
}

void Camera::upload() {
    data.view = glm::lookAt(data.position, data.position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
    auto w = static_cast<float>(width);
    auto h = static_cast<float>(height);
    data.project = glm::perspective(glm::radians(60.0f), w / h, 0.1f, 100.0f);
}

} // namespace wen