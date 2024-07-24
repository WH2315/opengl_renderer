#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace wen {

struct WindowInfo {
    std::string title;
    uint32_t width, height;
};

class Window final {
public:
    Window(const WindowInfo& info);
    ~Window();

    bool shouldClose() const;
    void pollEvents() const;

private:
    GLFWwindow* window_;
};

extern Window* g_window;

}