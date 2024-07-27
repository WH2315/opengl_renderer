#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 frag_position;
layout(location = 1) out vec3 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main() {
    gl_Position = project * view * model * vec4(position, 1.0);
    frag_position = vec3(model * vec4(position, 1.0));
    frag_normal = mat3(transpose(inverse(model))) * normal;
}