#include "resources/shader.hpp"
#include "base/utils.hpp"
#include "core/log.hpp"

namespace wen {

Shader::Shader(const std::string& filename, ShaderStage stage) {
    auto source = readFile(filename);
    const char* src = source.c_str();
    shader_ = glCreateShader(convert<GLenum>(stage));
    glShaderSource(shader_, 1, &src, nullptr);
    glCompileShader(shader_);
    checkCompileErrors(shader_);
}

Shader::~Shader() {
    glDeleteShader(shader_);
}

void Shader::checkCompileErrors(uint32_t shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        WEN_ERROR("Shader compilation failed: {0}", infoLog)
    }
}

} // namespace wen