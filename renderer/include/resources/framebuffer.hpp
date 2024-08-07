#pragma once

#include <cstdint>
#include <vector>

namespace wen {

enum class AttachmentType {
    eNone,
    eColor,
    eDepthStencil
};

struct AttachmentReference {
    AttachmentReference() = default;
    AttachmentReference(AttachmentType type) : type(type) {}

    AttachmentType type = AttachmentType::eNone;
};

struct Attachment {
    Attachment() = default;
    Attachment(std::initializer_list<AttachmentReference> references) : references(references) {}

    std::vector<AttachmentReference> references;
};

struct FramebufferAttachment {
    uint32_t width, height;
    Attachment attachment;
};

class Framebuffer {
public:
    Framebuffer(const FramebufferAttachment& framebuffer_attachment);
    ~Framebuffer();

    void invalidate();

    void bind() const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);
    int readPixel(uint32_t attachment_index, int x, int y);

    uint32_t getTexture(uint32_t attachment_index = 0) const {
        return color_attachment_indices_[attachment_index];
    }

    auto getFramebufferAttachment() const { return framebuffer_attachment_; }

private:
    uint32_t fbo_ = 0;
    FramebufferAttachment framebuffer_attachment_;

    std::vector<AttachmentReference> color_attachments_;
    AttachmentReference depth_attachment_;

    std::vector<uint32_t> color_attachment_indices_;
    uint32_t depth_attachment_index_;
};

} // namespace wen