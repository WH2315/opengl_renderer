#pragma once

namespace wen {

enum class ShaderStage {
    eVertex,
    eFragment
};

enum class VertexType {
    eFloat,
    eFloat2,
    eFloat3,
    eFloat4,
    eInt,
    eInt2,
    eInt3,
    eInt4,
    eBool
};

enum class IndexType {
    eUInt16,
    eUInt32
};

enum class TextureFilter {
    eNearest,
    eLinear,
    eLinearMipmapLinear,
};

enum class TextureWrap {
    eRepeat,
    eMirroredRepeat,
    eClampToEdge,
    eClampToBorder
};

} // namespace wen