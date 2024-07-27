#version 450 core

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 farg_normal;  

uniform vec3 light_position; 
uniform vec3 view_position;
uniform vec3 light_color;
uniform vec3 object_color;

out vec4 FragColor;

void main() {
    // 环境光 ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;
  	
    // 漫反射 diffuse 
    vec3 normal = normalize(farg_normal);
    vec3 light_dir = normalize(light_position - frag_position);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // 镜面反射 specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(view_position - frag_position);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
    FragColor = vec4(result, 1.0);
}