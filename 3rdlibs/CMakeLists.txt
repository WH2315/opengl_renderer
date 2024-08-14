add_subdirectory(spdlog)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
set(BUILD_SHARED_LIBS ON)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)

include(imgui.cmake)