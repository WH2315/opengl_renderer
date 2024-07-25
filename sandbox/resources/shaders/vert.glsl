#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex_coords;

uniform float x_offset;

layout(location = 0) out vec3 frag_color;
layout(location = 1) out vec2 frag_tex_coords;

void main() {
    gl_Position = vec4(position.x + x_offset, position.y, position.z, 1.0);
    frag_color = color;
    frag_tex_coords = tex_coords;
}