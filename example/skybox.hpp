#pragma once

#include "wen.hpp"
#include <stb_image.h>

class skybox {
public:
    uint32_t id;
    skybox(const std::shared_ptr<wen::ShaderProgram>& program) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        std::vector<std::string> faces {
            "example/resources/textures/skybox/right.jpg",
            "example/resources/textures/skybox/left.jpg",
            "example/resources/textures/skybox/top.jpg",
            "example/resources/textures/skybox/bottom.jpg",
            "example/resources/textures/skybox/front.jpg",
            "example/resources/textures/skybox/back.jpg"
        };

        stbi_set_flip_vertically_on_load(0);
        int width, height, channels;
        for (uint32_t i = 0; i < faces.size(); i++) {
            uint8_t* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                stbi_image_free(data);
                WEN_ERROR("Failed to load skybox texture: {0}", faces[i]);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        program->bind();
        program->setInt("skybox", 0);
        program->unbind();
    };
};