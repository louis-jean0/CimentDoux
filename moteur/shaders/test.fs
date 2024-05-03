#version 410 core

in vec3 normal;
in vec2 uvs;
in vec3 fragPos;

// Light
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float constant;
    float linear;
    float quadratic;
};

// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform vec3 viewPos;

#define NB_POINT_LIGHTS 1
uniform PointLight pointLights[NB_POINT_LIGHTS];
vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform Material material;

out vec4 FragColor;

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.0);
    for(int i = 0; i < NB_POINT_LIGHTS; ++i) {
        result += computePointLightsContribution(pointLights[i], normal, fragPos, viewDir);
    }
    FragColor = vec4(result,1.0);
}

vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Ambient
    vec3 ambient = pointLight.ambient * material.ambient * vec3(texture(texture_diffuse1, uvs));
    
    // Diffuse
    float diff = max(dot(normal, lightDir),0.0);
    vec3 diffuse = pointLight.diffuse * material.diffuse * diff * vec3(texture(texture_diffuse1, uvs));

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
    vec3 specular = pointLight.specular * material.specular * spec * vec3(texture(texture_diffuse1, uvs));

    // Attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * distance * distance));

    return (ambient + diffuse + specular);// * attenuation;

}