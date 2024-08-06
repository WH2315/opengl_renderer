#pragma once

#include "resources/geometry/geometry_buffer.hpp"

namespace wen {

class BoxGeometry : public GeometryBuffer {
public:
    BoxGeometry(float width = 1.0f, float height = 1.0f, float depth = 1.0f,
                float width_segments = 1.0f, float height_segments = 1.0f, float depth_segments = 1.0f) {
        width_segments = std::floor(width_segments);
        height_segments = std::floor(height_segments);
        depth_segments = std::floor(depth_segments);

        build(2, 1, 0, -1, -1, depth, height, width, depth_segments, height_segments);
        build(2, 1, 0, 1, -1, depth, height, -width, depth_segments, height_segments);

        build(0, 2, 1, 1, 1, width, depth, height, width_segments, depth_segments);
        build(0, 2, 1, 1, -1, width, depth, -height, width_segments, depth_segments);

        build(0, 1, 2, 1, -1, width, height, depth, width_segments, height_segments);
        build(0, 1, 2, -1, -1, width, height, -depth, width_segments, height_segments);

        this->upload();
    }

private:
    int number_of_vertices = 0;

    void build(int u, int v, int w, float udir, float vdir,
               float width, float height, float depth, float grid_x, float grid_y) {
        float width_half = width / 2.0f;
        float height_half = height / 2.0f;
        float depth_half = depth / 2.0f;

        float grid_x1 = grid_x + 1;
        float grid_y1 = grid_y + 1;

        float segment_width = width / grid_x;
        float segment_height = height / grid_y;

        Vertex vertex;
        glm::vec3 vector = glm::vec3(0.0f, 0.0f, 0.0f);

        uint32_t counter = 0;

        for (int iy = 0; iy < grid_y1; iy++) {
            float y = iy * segment_height - height_half;
            for (int ix = 0; ix < grid_x1; ix++) {
                float x = ix * segment_width - width_half;
                vector[u] = x * udir;
                vector[v] = y * vdir;
                vector[w] = depth_half;
                vertex.position = vector;
                vector[u] = 0.0f;
                vector[v] = 0.0f;
                vector[w] = depth > 0 ? 1.0f : -1.0f;
                vertex.normal = vector;                
                vertex.tex_coords = glm::vec2(ix / grid_x, 1.0f - (iy / grid_y));
                this->vertices.push_back(vertex);
                counter++;
            } 
        }

        for (int iy = 0; iy < grid_y; iy++) {
            for (int ix = 0; ix < grid_x; ix++) {
                int a = number_of_vertices + ix + grid_x1 * iy;
                int b = number_of_vertices + ix + grid_x1 * (iy + 1);
                int c = number_of_vertices + (ix + 1) + grid_x1 * (iy + 1);
                int d = number_of_vertices + (ix + 1) + grid_x1 * iy;

                this->indices.push_back(a);
                this->indices.push_back(b);
                this->indices.push_back(d);

                this->indices.push_back(b);
                this->indices.push_back(c);
                this->indices.push_back(d);
            }
        }

        number_of_vertices += counter;
    }
};

} // namespace wen