#version 450

layout(location = 0) in vec2 frag_tex_coords;

uniform sampler2D texture_normal1;

out vec4 FragColor;

void main() {
    FragColor = texture(texture_normal1, frag_tex_coords);
}