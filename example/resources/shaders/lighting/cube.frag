#version 450 core

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 farg_normal;  
layout(location = 2) in vec2 frag_tex_coords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cut_off;
    float outer_cut_off;
};

uniform Material material;
uniform DirLight dir_light;
uniform PointLight point_light;
uniform Spotlight spotlight;
uniform vec3 view_position;

out vec4 FragColor;

vec3 calcDirLight(DirLight dir_light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-dir_light.direction);

    // 漫反射
    float diff = max(dot(normal, light_dir), 0.0);

    // 镜面反射
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = vec3(texture(material.diffuse, frag_tex_coords)) * dir_light.ambient;
    vec3 diffuse = diff * vec3(texture(material.diffuse, frag_tex_coords)) * dir_light.diffuse;
    vec3 specular = spec * vec3(texture(material.specular, frag_tex_coords)) * dir_light.specular;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight point_light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(point_light.position - frag_position);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    float distance = length(point_light.position - frag_position);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance +  point_light.quadratic * (distance * distance));

    vec3 ambient = vec3(texture(material.diffuse, frag_tex_coords)) * point_light.ambient;
    vec3 diffuse = diff * vec3(texture(material.diffuse, frag_tex_coords)) * point_light.diffuse;
    vec3 specular = spec * vec3(texture(material.specular, frag_tex_coords)) * point_light.specular;

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotlight(Spotlight spotlight, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(spotlight.position - frag_position);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    float distance = length(spotlight.position - frag_position);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance +  spotlight.quadratic * (distance * distance));

    float theta = dot(light_dir, normalize(-spotlight.direction));
    float epsilon = spotlight.cut_off - spotlight.outer_cut_off;
    float intensity = clamp((theta - spotlight.outer_cut_off) / epsilon, 0.0, 1.0);

    vec3 ambient = vec3(texture(material.diffuse, frag_tex_coords)) * spotlight.ambient;
    vec3 diffuse = diff * vec3(texture(material.diffuse, frag_tex_coords)) * spotlight.diffuse;
    vec3 specular = spec * vec3(texture(material.specular, frag_tex_coords)) * spotlight.specular;

    ambient *= attenuation * intensity; 
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

void main() {
    vec3 normal = normalize(farg_normal);
    vec3 view_dir = normalize(view_position - frag_position);
    
    vec3 dir_light_result = calcDirLight(dir_light, normal, view_dir);
    vec3 point_light_result = calcPointLight(point_light, normal, view_dir);
    vec3 spotlight_result = calcSpotlight(spotlight, normal, view_dir);

    vec3 result = dir_light_result + point_light_result + spotlight_result;

    FragColor = vec4(result, 1.0);
}