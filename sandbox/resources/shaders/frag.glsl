#version 450 core

layout(location = 0) in vec2 frag_tex_coords;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(texture1, frag_tex_coords), texture(texture2, frag_tex_coords), 0.2);
}