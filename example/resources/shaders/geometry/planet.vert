#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coords;

layout(location = 0) out vec2 frag_tex_coords;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    mat4 project;
} camera;

uniform mat4 model;

void main() {
    gl_Position = camera.project * camera.view * model * vec4(position, 1.0);
    frag_tex_coords = tex_coords;
}