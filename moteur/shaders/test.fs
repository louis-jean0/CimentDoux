#version 410 core

// Entrées du vertex shader
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 uvs;
in vec3 fragPos;

// Structures de données des lumières
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float constant;
    float linear;
    float quadratic;
};

struct TorchLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    vec3 direction;
    float far_plane;
    float cut_off;
    float outer_cut_off;
    mat4 light_space_matrix;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
};

uniform bool hasDiffuse;
uniform sampler2D texture_diffuse1;
uniform bool hasSpecular;
uniform sampler2D texture_specular1;
uniform bool hasNormalMap;
uniform sampler2D normal_map1;

#define NB_POINT_LIGHTS_MAX 30
uniform int nb_point_lights;
uniform PointLight pointLights[NB_POINT_LIGHTS_MAX]; // Also contains torch lights

#define NB_TORCH_LIGHTS_MAX 10
uniform int nb_torch_lights;
uniform TorchLight torchLights[NB_TORCH_LIGHTS_MAX];
uniform sampler2D shadow_map[NB_TORCH_LIGHTS_MAX];
uniform int shadow_map_indices[NB_TORCH_LIGHTS_MAX];

uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;

// Function prototypes
vec3 computeDirectionalLightContribution(DirectionalLight directionalLight, vec3 normal, vec3 viewDir);
vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 computeTorchLightsContribution(TorchLight torchLight, vec3 normal, vec3 fragPos, vec3 viewDir, int shadow_map_index);
vec3 disturbNormalWithNormalMap(vec3 tangent, vec3 bitangent, vec3 normal);
float shadowCalculation(TorchLight torchLight, vec3 normal, vec3 fragPos, int shadow_map_index);

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 norm = vec3(0.0, 0.0, 0.0);
    vec3 result = vec3(0.0, 0.0, 0.0);

    if(hasNormalMap) {
        norm = disturbNormalWithNormalMap(tangent, bitangent, normal);
    } else {
        norm = normal;
    }

    result += computeDirectionalLightContribution(directionalLight, norm, viewDir);

    for(int i = 0; i < nb_point_lights; ++i) {
        result += computePointLightsContribution(pointLights[i], norm, fragPos, viewDir);
    }

    for(int i = 0; i < nb_torch_lights; ++i) {
        result += computeTorchLightsContribution(torchLights[i], norm, fragPos, viewDir, shadow_map_indices[i]);
    }

    result += material.emissive;
    // vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // projCoords = projCoords * 0.5 + 0.5;
    // float depth = texture(shadow_map[0], uvs).r;
    // FragColor = vec4(depth, depth, depth, 1.0);
    //FragColor = vec4(projCoords, 1.0);
    FragColor = vec4(result, 1.0);
}

vec3 computeDirectionalLightContribution(DirectionalLight directionalLight, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-directionalLight.direction);

    vec3 ambient = directionalLight.ambient * material.ambient;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = hasDiffuse ? directionalLight.diffuse * material.diffuse * diff * vec3(texture(texture_diffuse1, uvs)) : directionalLight.diffuse * material.diffuse * diff;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = hasSpecular ? directionalLight.specular * material.specular * spec * vec3(texture(texture_specular1, uvs)) : directionalLight.specular * material.specular * spec;

    return ambient + diffuse + specular;
}

vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Ambient
    vec3 ambient = pointLight.ambient * material.ambient;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse;
    if (hasDiffuse) {
        diffuse = pointLight.diffuse * material.diffuse * diff * vec3(texture(texture_diffuse1, uvs));
    } else {
        diffuse = pointLight.diffuse * material.diffuse * diff;
    }

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular;
    if (hasSpecular) {
        specular = pointLight.specular * material.specular * spec * vec3(texture(texture_specular1, uvs));
    } else {
        specular = pointLight.specular * material.specular * spec;
    }

    // Attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * distance * distance));
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 computeTorchLightsContribution(TorchLight torchLight, vec3 normal, vec3 fragPos, vec3 viewDir, int shadow_map_index) {
    vec3 lightDir = normalize(torchLight.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Ambient
    vec3 ambient = torchLight.ambient * material.ambient;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse;
    if (hasDiffuse) {
        diffuse = torchLight.diffuse * material.diffuse * diff * vec3(texture(texture_diffuse1, uvs));
    } else {
        diffuse = torchLight.diffuse * material.diffuse * diff;
    }

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular;
    if (hasSpecular) {
        specular = torchLight.specular * material.specular * spec * vec3(texture(texture_specular1, uvs));
    } else {
        specular = torchLight.specular * material.specular * spec;
    }

    // Attenuation
    float distance = length(torchLight.position - fragPos);
    float attenuation = 1.0 / (torchLight.constant + (torchLight.linear * distance) + (torchLight.quadratic * distance * distance));

    // Torch light caracteristics
    float theta = dot(lightDir, normalize(-torchLight.direction));
    float epsilon = torchLight.cut_off - torchLight.outer_cut_off;
    float intensity = clamp((theta - torchLight.outer_cut_off) / epsilon, 0.0, 1.0);
    attenuation *= intensity;
    float shadow = shadowCalculation(torchLight, normal, fragPos, shadow_map_index);
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

vec3 disturbNormalWithNormalMap(vec3 tangent, vec3 bitangent, vec3 normal) {
    vec3 T = normalize(tangent);
    vec3 B = normalize(bitangent);
    vec3 N = normalize(normal);
    mat3 TBN = transpose(mat3(T, B, N));
    vec3 normal_map_normal = texture(normal_map1, uvs).xyz * 2.0 - 1.0;
    return normalize(TBN * normal_map_normal);
}

float shadowCalculation(TorchLight torchLight, vec3 normal, vec3 fragPos, int shadow_map_index) {
    vec4 fragPosLightSpace = torchLight.light_space_matrix * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) {
        return 0.0; 
    }

    float closestDepth = texture(shadow_map[shadow_map_index], projCoords.xy).r * torchLight.far_plane;
    float currentDepth = projCoords.z * torchLight.far_plane;

    vec3 lightDir = normalize(torchLight.position - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map[shadow_map_index], 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadow_map[shadow_map_index], projCoords.xy + vec2(x, y) * texelSize).r * torchLight.far_plane;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}