#pragma once

#include "resources/geometry/geometry_buffer.hpp"
#include <glm/gtc/constants.hpp>

namespace wen {

class TorusGeometry : public GeometryBuffer {
public:
    TorusGeometry(float radius = 1.0f, float tube = 0.4f,
                  float radial_segments = 12.0f, float tubular_segments = 48.0f,
                  float arc = glm::pi<float>() * 2.0f) {
        radial_segments = std::floor(radial_segments);
        tubular_segments = std::floor(tubular_segments);

        Vertex vertex;

        for (uint32_t j = 0; j <= radial_segments; j++) {
            float v = j / radial_segments * glm::pi<float>() * 2.0f;
            for (uint32_t i = 0; i <= tubular_segments; i++) {
                float u = i / tubular_segments * arc;
                vertex.position = {
                    (radius + tube * glm::cos(v)) * glm::cos(u),
                    (radius + tube * glm::cos(v)) * glm::sin(u),
                    tube * glm::sin(v)
                };
                glm::vec3 center = glm::vec3(radius * glm::cos(u), radius * glm::sin(u), 0.0f);
                vertex.normal = glm::normalize(vertex.position - center);
                vertex.tex_coords = glm::vec2(i / tubular_segments, j / radial_segments);
                this->vertices.push_back(vertex);
            }
        }

        for (uint32_t j = 1; j <= radial_segments; j++) {
            for (uint32_t i = 1; i <= tubular_segments; i++) {
                uint32_t a = (tubular_segments + 1) * j + i - 1;
                uint32_t b = (tubular_segments + 1) * (j - 1) + i - 1;
                uint32_t c = (tubular_segments + 1) * (j - 1) + i;
                uint32_t d = (tubular_segments + 1) * j + i;

                this->indices.push_back(a);
                this->indices.push_back(b);
                this->indices.push_back(d);

                this->indices.push_back(b);
                this->indices.push_back(c);
                this->indices.push_back(d);
            }
        }

        this->upload();
    }
};

} // namespace wen