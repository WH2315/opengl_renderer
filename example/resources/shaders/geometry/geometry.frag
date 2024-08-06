#version 450

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 frag_normal;
layout(location = 2) in vec2 frag_tex_coords;

uniform sampler2D texture1;

uniform vec3 view_position;
uniform vec3 light_position;

out vec4 FragColor;

void main() {
    FragColor = texture(texture1, frag_tex_coords);
}