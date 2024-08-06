#pragma once

#include "resources/geometry/geometry_buffer.hpp"
#include <glm/gtc/constants.hpp>

namespace wen {

class SphereGeometry : public GeometryBuffer {
public:
    SphereGeometry(float radius = 1.0f,
                   float width_segments = 64.0f, float height_segments = 32.0f,
                   float phi_start = 0.0f, float phi_length = glm::pi<float>() * 2.0f,
                   float theta_start = 0.0f, float theta_length = glm::pi<float>()) {
        const float theta_end = glm::min(theta_start + theta_length, glm::pi<float>());
        int index = 0;

        width_segments = glm::max(3.0f, width_segments);
        height_segments = glm::max(2.0f, height_segments);

        std::vector<std::vector<int>> grid;

        Vertex vertex;

        for (uint32_t iy = 0; iy <= height_segments; iy++) {
            std::vector<int> vertices_row;

            float v = iy / height_segments;
            float u_offset = 0.0f;
            if (iy == 0 && theta_start == 0.0f) {
                u_offset = 0.5f / width_segments;
            } else if (iy == height_segments && theta_end == glm::pi<float>()) {
                u_offset = -0.5f / width_segments;
            }

            for (uint32_t ix = 0; ix <= width_segments; ix++) {
                float u = ix / width_segments;
                vertex.position = {
                    -radius * glm::cos(phi_start + u * phi_length) * glm::sin(theta_start + v * theta_length),
                    radius * glm::cos(theta_start + v * theta_length),
                    radius * glm::sin(phi_start + u * phi_length) * glm::sin(theta_start + v * theta_length)
                };
                vertex.normal = glm::normalize(vertex.position);
                vertex.tex_coords = glm::vec2(u + u_offset, 1 - v);
                this->vertices.push_back(vertex);
                vertices_row.push_back(index++);
            }
            grid.push_back(vertices_row);
        }

        for (uint32_t iy = 0; iy < height_segments; iy++) {
            for (uint32_t ix = 0; ix < width_segments; ix++) {
                uint32_t a = grid[iy][ix + 1];
                uint32_t b = grid[iy][ix];
                uint32_t c = grid[iy + 1][ix];
                uint32_t d = grid[iy + 1][ix + 1];

                if (iy != 0 || theta_start > 0.0f) {
                    this->indices.push_back(a);
                    this->indices.push_back(b);
                    this->indices.push_back(d);
                }
                if (iy != height_segments - 1 || theta_end < glm::pi<float>()) {
                    this->indices.push_back(b);
                    this->indices.push_back(c);
                    this->indices.push_back(d);
                }
            }
        }

        this->upload();
    }
};

} // namespace wen