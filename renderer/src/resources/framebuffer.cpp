#include "resources/framebuffer.hpp"
#include <core/log.hpp>
#include <glad/glad.h>

namespace wen {

Framebuffer::Framebuffer(const FramebufferAttachment& framebuffer_attachment)
    : framebuffer_attachment_(framebuffer_attachment) {
    for (auto ref : framebuffer_attachment_.attachment.references) {
        if (ref.type != AttachmentType::eDepthStencil) {
            color_attachments_.emplace_back(ref);
        } else {
            depth_attachment_ = ref;
        }
    }
    invalidate();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(color_attachment_indices_.size(), color_attachment_indices_.data());
    glDeleteRenderbuffers(1, &depth_attachment_index_);
}

void Framebuffer::invalidate() {
    if (fbo_) {
        glDeleteFramebuffers(1, &fbo_);
        glDeleteTextures(color_attachment_indices_.size(), color_attachment_indices_.data());
        glDeleteRenderbuffers(1, &depth_attachment_index_);
        color_attachment_indices_.clear();
        depth_attachment_index_ = 0;
    }

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    auto w = framebuffer_attachment_.width, h = framebuffer_attachment_.height;
    if (!color_attachments_.empty()) {
        color_attachment_indices_.resize(color_attachments_.size());
        glGenTextures(color_attachments_.size(), color_attachment_indices_.data());
        for (size_t i = 0; i < color_attachment_indices_.size(); i++) {
            auto texture = color_attachment_indices_[i];
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
        }
    }

    if (depth_attachment_.type != AttachmentType::eNone) {
        glGenRenderbuffers(1, &depth_attachment_index_);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_attachment_index_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_attachment_index_);
    }

    if (color_attachment_indices_.size() > 1) {
        if (color_attachment_indices_.size() > 4) {
            WEN_ERROR("We only support 4 color attachments!")
        }
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(color_attachment_indices_.size(), buffers);
    } else if (color_attachment_indices_.empty()) {
        glDrawBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        WEN_ERROR("Framebuffer is not complete!")
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, framebuffer_attachment_.width, framebuffer_attachment_.height);
}

void Framebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::resize(uint32_t width, uint32_t height) {
    framebuffer_attachment_.width = width;
    framebuffer_attachment_.height = height;
    invalidate();
}

int Framebuffer::readPixel(uint32_t attachment_index, int x, int y) {
    int value;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_INT, &value);
    return value;
}

} // namespace wen