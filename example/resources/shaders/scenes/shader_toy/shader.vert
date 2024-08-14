#version 450

layout(location = 0) in vec2 pos;

layout(location = 0) out vec2 frag_coord;

uniform float scene_width;
uniform float scene_height;

void main() {
    gl_Position = vec4(pos, 0, 1);
    frag_coord = ((pos + 1) / 2) * vec2(scene_width, scene_height);
}