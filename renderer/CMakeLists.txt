project(renderer)

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_library(${PROJECT_NAME} STATIC ${SRC} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ./include ../3rdlibs/stb_image)

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog glfw glad glm imgui assimp)

function(COPYDLL TARGET_NAME PATH_PREFIX)
    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PATH_PREFIX}/3rdlibs/assimp/bin/assimpd.dll
        $<TARGET_FILE_DIR:${TARGET_NAME}>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction(COPYDLL)