#version 450

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 frag_normal;
layout(location = 2) in vec2 frag_tex_coords;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 view_position;
uniform PointLight light;
uniform DirLight dir_light;

out vec4 FragColor;

vec3 calcDirLight(DirLight dir_light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-dir_light.direction);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);

    vec3 ambient = vec3(texture(texture_diffuse1, frag_tex_coords)) * dir_light.ambient;
    vec3 diffuse = diff * vec3(texture(texture_diffuse1, frag_tex_coords)) * dir_light.diffuse;
    vec3 specular = spec * vec3(texture(texture_specular1, frag_tex_coords)) * dir_light.specular;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - frag_position);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);

    float distance = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance +  light.quadratic * (distance * distance));

    vec3 ambient = vec3(texture(texture_diffuse1, frag_tex_coords)) * light.ambient;
    vec3 diffuse = diff * vec3(texture(texture_diffuse1, frag_tex_coords)) * light.diffuse;
    vec3 specular = spec * vec3(texture(texture_specular1, frag_tex_coords)) * light.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main() {
    vec3 view_dir = normalize(view_position - frag_position);
    vec3 normal = normalize(frag_normal);

    vec3 point_light_result = calcPointLight(light, normal, view_dir);
    vec3 dir_light_result = calcDirLight(dir_light, normal, view_dir);

    vec3 result = point_light_result + dir_light_result;

    FragColor = vec4(result, 1.0);
}