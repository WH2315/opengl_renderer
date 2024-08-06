#pragma once

#include "resources/geometry/geometry_buffer.hpp"

namespace wen {

class PlaneGeometry : public GeometryBuffer {
public:
    PlaneGeometry(float width = 1.0f, float height = 1.0f, float width_segments = 1.0f, float height_segments = 1.0f) {
        float width_half = width / 2.0f;
        float height_half = height / 2.0f;

        float grid_x = std::floor(width_segments);        
        float grid_y = std::floor(height_segments);        
        float grid_x1 = grid_x + 1;
        float grid_y1 = grid_y + 1;

        float segment_width = width / width_segments;
        float segment_height = height / height_segments;

        Vertex vertex;

        for (int iy = 0; iy < grid_y1; iy++) {
            float y = iy * segment_height - height_half;
            for (int ix = 0; ix < grid_x1; ix++) {
                float x = ix * segment_width - width_half;
                vertex.position = glm::vec3(x, -y, 0.0f);
                vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);                
                vertex.tex_coords = glm::vec2(ix / width_segments, 1.0f - (iy / height_segments));
                this->vertices.push_back(vertex);
            } 
        }

        for (int iy = 0; iy < grid_y; iy++) {
            for (int ix = 0; ix < grid_x; ix++) {
                int a = ix + grid_x1 * iy;
                int b = ix + grid_x1 * (iy + 1);
                int c = (ix + 1) + grid_x1 * (iy + 1);
                int d = (ix + 1) + grid_x1 * iy;

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