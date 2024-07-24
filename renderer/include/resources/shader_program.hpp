#pragma once

#include "resources/shader.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace wen {

class ShaderProgram final {
public:
    ShaderProgram();
    ~ShaderProgram();

    ShaderProgram& attach(const std::shared_ptr<Shader>& shader);
    ShaderProgram& link();

    void bind() const;
    void unbind() const;

    ShaderProgram& setBool(const std::string& name, bool value);
    ShaderProgram& setInt(const std::string& name, int value);
    ShaderProgram& setIntArray(const std::string& name, int* values, uint32_t count);
    ShaderProgram& setFloat(const std::string& name, float value);
    ShaderProgram& setVec2(const std::string& name, const glm::vec2& value);
    ShaderProgram& setVec3(const std::string& name, const glm::vec3& value);
    ShaderProgram& setVec4(const std::string& name, const glm::vec4& value);
    ShaderProgram& setMat2(const std::string& name, const glm::mat2& value);
    ShaderProgram& setMat3(const std::string& name, const glm::mat3& value);
    ShaderProgram& setMat4(const std::string& name, const glm::mat4& value);

private:
    void checkLinkErrors(uint32_t program);

private:
    uint32_t program_;
};

} // namespace wen