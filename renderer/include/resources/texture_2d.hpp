#pragma once

#include "base/enums.hpp"
#include <string>
#include <glad/glad.h>

namespace wen {

struct TextureInfos {
    TextureFilter min_filter = TextureFilter::eLinear;
    TextureFilter mag_filter = TextureFilter::eLinear;
    TextureWrap wrap_s = TextureWrap::eRepeat;
    TextureWrap wrap_t = TextureWrap::eRepeat;
    bool gamma = false;
};

class Texture2D final {
public:
    Texture2D(const std::string& filename, const TextureInfos& infos = {});
    Texture2D(uint32_t width, uint32_t height);
    ~Texture2D();

    void bind(uint32_t slot = 0) const;

    void setData(const void* data);

    uint32_t getTexture() const { return texture_; }

private:
    uint32_t texture_;
    uint32_t width_, height_;
    GLint internal_format_ = 0;
    GLenum data_format_ = 0;
};

} // namespace wen