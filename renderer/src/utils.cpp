#include "base/utils.hpp"
#include "base/enums.hpp"
#include "core/log.hpp"
#include <fstream>

namespace wen {

std::string readFile(const std::string& filename) {
    std::string result;
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        } else {
            WEN_ERROR("Could not read file '{0}'", filename)
        }
    } else {
        WEN_ERROR("Could not open file '{0}'", filename)
    }
    return result;
}

template <>
GLenum convert(ShaderStage stage) {
    switch (stage) {
        case ShaderStage::eVertex:
            return GL_VERTEX_SHADER;
        case ShaderStage::eFragment:
            return GL_FRAGMENT_SHADER;
        default:
            WEN_ERROR("Invalid shader stage")
            return 0;
    }
}

template <>
GLenum convert(VertexType type) {
    switch (type) {
        case VertexType::eFloat:
        case VertexType::eFloat2:
        case VertexType::eFloat3:
        case VertexType::eFloat4: return GL_FLOAT;
        case VertexType::eInt:
        case VertexType::eInt2:
        case VertexType::eInt3:
        case VertexType::eInt4:   return GL_INT;
        case VertexType::eBool:   return GL_BOOL;
        default:                  return GL_NONE;
    }
}

template <>
GLint convert(VertexType type) {
    switch (type) {
        case VertexType::eFloat:  return 4 * 1;
        case VertexType::eFloat2: return 4 * 2;
        case VertexType::eFloat3: return 4 * 3;
        case VertexType::eFloat4: return 4 * 4;
        case VertexType::eInt:    return 4 * 1;
        case VertexType::eInt2:   return 4 * 2;
        case VertexType::eInt3:   return 4 * 3;
        case VertexType::eInt4:   return 4 * 4;
        case VertexType::eBool:   return 1;
        default:                  return 0;
    }
}

} // namespace wen