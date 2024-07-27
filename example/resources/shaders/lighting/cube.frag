#version 450 core

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 farg_normal;  

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 view_position;

out vec4 FragColor;

void main() {
    vec3 ambient = material.ambient * light.ambient;
  	
    vec3 normal = normalize(farg_normal);
    vec3 light_dir = normalize(light.position - frag_position);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    vec3 view_dir = normalize(view_position - frag_position);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}