#version 450

layout(location = 0) in vec2 frag_tex_coords;

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main() {
    FragColor = texture(texture_diffuse1, frag_tex_coords);
}