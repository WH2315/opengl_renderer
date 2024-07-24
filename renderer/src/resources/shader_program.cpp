#include "resources/shader_program.hpp"
#include "base/utils.hpp"
#include "core/log.hpp"

namespace wen {

ShaderProgram::ShaderProgram() {
    program_ = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_);
}

ShaderProgram& ShaderProgram::attach(const std::shared_ptr<Shader>& shader) {
    glAttachShader(program_, shader->shader_);
    return *this;
}

ShaderProgram& ShaderProgram::link() {
    glLinkProgram(program_);
    checkLinkErrors(program_);
    return *this;
}

void ShaderProgram::bind() const {
    glUseProgram(program_);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}

void ShaderProgram::checkLinkErrors(uint32_t program) {
    int success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        WEN_ERROR("Shader program compilation failed: {0}", infoLog)
    }
}

} // namespace wen