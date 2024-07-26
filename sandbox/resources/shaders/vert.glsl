#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

layout(location = 0) out vec2 frag_tex_coords;

void main() {
    gl_Position =  project * view * model * vec4(position.x, position.y, position.z, 1.0);
    frag_tex_coords = tex_coords;
}