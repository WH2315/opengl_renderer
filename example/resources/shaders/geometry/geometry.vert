#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coords;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    mat4 project;
} camera;

uniform mat4 model;

layout(location = 0) out vec3 frag_position;
layout(location = 1) out vec3 frag_normal;
layout(location = 2) out vec2 frag_tex_coords;

void main() {
    gl_Position = camera.project * camera.view * model * vec4(position, 1.0);
    frag_position = vec3(model * vec4(position, 1.0));
    frag_normal = mat3(transpose(inverse(model))) * normal;
    frag_tex_coords = tex_coords;
}