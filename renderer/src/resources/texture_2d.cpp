#include "resources/texture_2d.hpp"
#include "base/utils.hpp"
#include "core/log.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace wen {

Texture2D::Texture2D(const std::string& filename, const TextureInfos& infos) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        WEN_ERROR("Failed to load texture: {0}", filename)
        return;
    }
    width_ = width;
    height_ = height;
    GLenum internal_format = 0, data_format = 0;
    if (channels == 4) {
        internal_format = GL_RGBA8;
        data_format = GL_RGBA;
    } else if (channels == 3) {
        internal_format = GL_RGB8;
        data_format = GL_RGB;
    }
    internal_format_ = internal_format;
    data_format_ = data_format;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
    glTextureStorage2D(texture_, 1, internal_format_, width_, height_);
    glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, convert<GLenum>(infos.min_filter));
    glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, convert<GLenum>(infos.mag_filter));
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_S, convert<GLenum>(infos.wrap_s));
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_T, convert<GLenum>(infos.wrap_t));
    glTextureSubImage2D(texture_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture_);
    stbi_image_free(data);
}

Texture2D::Texture2D(uint32_t width, uint32_t height) : width_(width), height_(height) {
    internal_format_ = GL_RGBA8;
    data_format_ = GL_RGBA;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_);
    glTextureStorage2D(texture_, 1, internal_format_, width_, height_);
    glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &texture_);
}

void Texture2D::bind(uint32_t slot) const {
    glBindTextureUnit(slot, texture_);
}

void Texture2D::setData(const void* data) {
    uint32_t bpp = data_format_ == GL_RGBA ? 4 : 3;
    WEN_DEBUG("texture size: {0}", width_ * height_ * bpp)
    glTextureSubImage2D(texture_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
}

} // namespace wen