add_subdirectory(spdlog)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(assimp)

include(imgui.cmake)