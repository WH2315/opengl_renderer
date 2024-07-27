#include "core/window.hpp"
#include "core/log.hpp"

namespace wen {

Window* g_window = nullptr;

Window::Window(const WindowInfo& info) {
    WEN_INFO("Create window:({0}, {1}, {2})", info.title, info.width, info.height)
    data_.width = info.width;
    data_.height = info.height;

    if (!glfwInit()) {
        WEN_ERROR("Failed to init glfw!")
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(info.width, info.height, info.title.c_str(), nullptr, nullptr);
    if (!window_) {
        WEN_ERROR("Failed to create window!")
        glfwTerminate();
    }
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        WEN_ERROR("Failed to init glad!")
    }

    WEN_DEBUG(" OpenGL Vendor:   {0}", (char*)glGetString(GL_VENDOR))
    WEN_DEBUG(" OpenGL Renderer: {0}", (char*)glGetString(GL_RENDERER))
    WEN_DEBUG(" OpenGL Version:  {0}", (char*)glGetString(GL_VERSION))

    glfwSwapInterval(0);

    glfwSetWindowUserPointer(window_, &data_);

    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;
        glViewport(0, 0, width, height);
    });
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::pollEvents() const {
    glfwPollEvents();
    glfwSwapBuffers(window_);
}

Window::~Window() {
    glfwDestroyWindow(window_);
    window_ = nullptr;
    glfwTerminate();
}

} // namespace wen