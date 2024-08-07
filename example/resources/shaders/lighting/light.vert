#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main() {
    gl_Position = project * view * model * vec4(position, 1.0);
}