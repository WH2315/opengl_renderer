#version 450

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 frag_normal;

out vec4 FragColor;

uniform int mode;
uniform vec3 view_position;
uniform samplerCube skybox;

void main() {
    vec3 I = normalize(frag_position - view_position);
    if (mode == 0) {
        vec3 R = reflect(I, normalize(frag_normal));
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    } else {
        float ratio = 1.00 / 1.52;
        vec3 R = refract(I, normalize(frag_normal), ratio);
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
}