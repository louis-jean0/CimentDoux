#version 410 core

in vec3 normal;
in vec2 uvs;
in vec3 fragPos;

// PointLight
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float constant;
    float linear;
    float quadratic;
    bool is_torch_light;
    vec3 direction;
    float cut_off;
    float outer_cut_off;
};

// DirectionalLight
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
};

uniform bool hasDiffuse;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

uniform bool hasSpecular;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform vec3 viewPos;

#define NB_POINT_LIGHTS_MAX 20
uniform int nb_point_lights;
uniform PointLight pointLights[NB_POINT_LIGHTS_MAX];
vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform DirectionalLight directionalLight;
vec3 computeDirectionalLightContribution(DirectionalLight directionalLight, vec3 normal, vec3 viewDir);

uniform Material material;

out vec4 FragColor;

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = computeDirectionalLightContribution(directionalLight, normal, viewDir);
    for(int i = 0; i < nb_point_lights - 1; ++i) {
        result += computePointLightsContribution(pointLights[i], normal, fragPos, viewDir);
    }
    result += material.emissive;
    vec3 norm = normalize(normal);
    //(norm + 1.0) * 0.5
    FragColor = vec4(result,1.0);
}

vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Ambient
    vec3 ambient = pointLight.ambient * material.ambient;
    
    // Diffuse
    float diff = max(dot(normal, lightDir),0.0);
    vec3 diffuse;
    if(hasDiffuse) {
        diffuse = pointLight.diffuse * material.diffuse * diff * vec3(texture(texture_diffuse1, uvs));
    }
    else {
        diffuse = pointLight.diffuse * material.diffuse * diff;
    }

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
    vec3 specular;
    if(hasSpecular) {
        specular = pointLight.specular * material.specular * spec * vec3(texture(texture_specular1, uvs));
    }
    else {
        specular = pointLight.specular * material.specular * spec;
    }

    // Attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * distance * distance));

    if(pointLight.is_torch_light) {
        float theta = dot(lightDir, normalize(-pointLight.direction));
        float epsilon = pointLight.cut_off - pointLight.outer_cut_off;
        float intensity = clamp((theta - pointLight.outer_cut_off) / epsilon, 0.0, 1.0);
        attenuation *= intensity;
    }

    return (ambient + diffuse + specular) * attenuation;
}

vec3 computeDirectionalLightContribution(DirectionalLight directionalLight, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-directionalLight.direction); // - because we specify a direction from the light to the scene and not the other way around
    
    // Ambient
    vec3 ambient = directionalLight.ambient * material.ambient * vec3(texture(texture_diffuse1, uvs));

    // Diffuse
    float diff = max(dot(normal, lightDir),0.0);
    vec3 diffuse;
    if(hasDiffuse) {
        diffuse = directionalLight.diffuse * material.diffuse * diff * vec3(texture(texture_diffuse1, uvs));
    }
    else {
        diffuse = directionalLight.diffuse * material.diffuse * diff;
    }

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
    vec3 specular;
    if(hasSpecular) {
        specular = directionalLight.specular * material.specular * spec * vec3(texture(texture_specular1,uvs));
    }
    else {
        specular = directionalLight.specular * material.specular * spec;
    }

    return ambient + diffuse + specular;
}