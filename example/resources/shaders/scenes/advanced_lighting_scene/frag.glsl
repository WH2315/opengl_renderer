#version 450

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 frag_normal;
layout(location = 2) in vec2 frag_tex_coords;

layout(location = 0) out vec4 FragColor;

uniform sampler2D wood;
uniform vec3 view_pos;
uniform vec3 light_pos[4];
uniform vec3 light_color[4];
uniform bool gamma;

vec3 BlinnPhong(vec3 light_pos, vec3 light_color) {
    vec3 normal = normalize(frag_normal);

    vec3 light_dir = normalize(light_pos - frag_position);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 view_dir = normalize(view_pos - frag_position);
    vec3 H = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, H), 0.0), 64);
    vec3 specular = spec * light_color;

    float max_distance = 1.5;
    float distance = length(light_pos - frag_position);
    float attenuation = 1.0 / (gamma ? distance * distance : distance);

    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main() {
    vec3 color = texture(wood, frag_tex_coords).rgb;

    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 4; i++) {
        lighting += BlinnPhong(light_pos[i], light_color[i]);
    }

    color *= lighting;

    if (gamma) {
        color = pow(color, vec3(1.0 / 2.2));
    }

    FragColor = vec4(color, 1.0);
}