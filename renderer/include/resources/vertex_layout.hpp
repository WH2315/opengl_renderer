#pragma once

#include <base/enums.hpp>
#include <base/utils.hpp>
#include <vector>

namespace wen {

struct VertexElement {
    std::string name;
    GLenum type;
    GLint size;
    GLint offset;
    bool normalized;

    VertexElement(const std::string& name, VertexType type, bool normalized = false)
        : name(name),
          type(convert<GLenum>(type)),
          size(convert<GLint>(type)),
          offset(0),
          normalized(normalized){};
};

class VertexLayout final {
    friend class VertexArray;

public:
    VertexLayout() = default;
    ~VertexLayout() = default;

    VertexLayout(const std::initializer_list<VertexElement>& elements) : elements_(elements) {
        for (auto& element : elements_) {
            element.offset = stride_;
            stride_ += element.size;
        }
    }

    std::vector<VertexElement>::iterator begin() { return elements_.begin(); }
    std::vector<VertexElement>::iterator end() { return elements_.end(); }
    std::vector<VertexElement>::const_iterator begin() const { return elements_.begin(); }
    std::vector<VertexElement>::const_iterator end() const { return elements_.end(); }

private:
    std::vector<VertexElement> elements_;
    GLsizei stride_ = 0;
};

} // namespace wen