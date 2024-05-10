#version 410 core

in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
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

uniform bool hasSpecular;
uniform sampler2D texture_specular1;

uniform bool hasNormalMap;
uniform sampler2D normal_map1;
vec3 disturbNormalWithNormalMap(vec3 tangent, vec3 bitangent, vec3 normal);

#define NB_POINT_LIGHTS_MAX 30
uniform int nb_point_lights;
uniform PointLight pointLights[NB_POINT_LIGHTS_MAX];
vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform DirectionalLight directionalLight;
vec3 computeDirectionalLightContribution(DirectionalLight directionalLight, vec3 normal, vec3 viewDir);

uniform Material material;

uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 norm = vec3(0.0,0.0,0.0);
    vec3 result = vec3(0.0,0.0,0.0);
    if(hasNormalMap) {
        norm = disturbNormalWithNormalMap(tangent, bitangent, normal);
    }
    else {
        norm = normal;
    }
    result += computeDirectionalLightContribution(directionalLight, norm, viewDir);
    for(int i = 0; i < nb_point_lights; ++i) {
        result += computePointLightsContribution(pointLights[i], norm, fragPos, viewDir);
    }
    result += material.emissive;
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
    vec3 ambient = directionalLight.ambient * material.ambient ;

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

vec3 disturbNormalWithNormalMap(vec3 tangent, vec3 bitangent, vec3 normal) {
    vec3 T = normalize(tangent); // Already done normal_matrix * normal in vertex shader
    vec3 B = normalize(bitangent); // Already done normal_matrix * normal in vertex shader
    vec3 N = normalize(normal);  // Already done normal_matrix * normal in vertex shader
    mat3 TBN = transpose(mat3(T,B,N));
    vec3 normal_map_normal = texture(normal_map1, uvs).xyz;
    normal_map_normal = normal_map_normal * 2.0 - 1.0;
    vec3 disturbed_normal = normalize(TBN * normal_map_normal);
    return disturbed_normal;
}