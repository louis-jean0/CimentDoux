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

#define NB_POINT_LIGHTS_MAX 20
uniform int nb_point_lights;
uniform PointLight pointLights[NB_POINT_LIGHTS_MAX];
uniform mat4 light_projection;
uniform mat4 light_view;

vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir, int shadow_map_index);

uniform DirectionalLight directionalLight;
vec3 computeDirectionalLightContribution(DirectionalLight directionalLight, vec3 normal, vec3 viewDir);

uniform sampler2D shadow_map[NB_POINT_LIGHTS_MAX];
uniform int shadow_map_indices[NB_POINT_LIGHTS_MAX];
float shadowCalculation(PointLight pointLight, vec3 fragPos, int shadow_map_index);

uniform Material material;

uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 norm = vec3(0.0,0.0,0.0);
    vec3 result = vec3(0.0,0.0,0.0);
    float shadow = 0.0;
    if(hasNormalMap) {
        norm = disturbNormalWithNormalMap(tangent, bitangent, normal);
    }
    else {
        norm = normal;
    }
    result += computeDirectionalLightContribution(directionalLight, norm, viewDir);
    for(int i = 0; i < nb_point_lights; ++i) {
        result += computePointLightsContribution(pointLights[i], norm, fragPos, viewDir, shadow_map_indices[i]);
    }
    result += material.emissive;
    FragColor = vec4(result,1.0);
}

vec3 computePointLightsContribution(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir, int shadow_map_index) {
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

    // Torch light
    if(pointLight.is_torch_light) {
        float theta = dot(lightDir, normalize(-pointLight.direction));
        float epsilon = pointLight.cut_off - pointLight.outer_cut_off;
        float intensity = clamp((theta - pointLight.outer_cut_off) / epsilon, 0.0, 1.0);
        attenuation *= intensity;
        // Shadow only for torch light to optimize performances
        float shadow = shadowCalculation(pointLight, fragPos, shadow_map_index);
        return (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
    }
    else {
        //return (ambient + diffuse + specular) * attenuation;    
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

float shadowCalculation(PointLight pointLight, vec3 fragPos, int shadow_map_index) {
    vec4 fragPos_light_space = light_projection * light_view * vec4(fragPos, 1.0);
    fragPos_light_space.xyz /= fragPos_light_space.w;
    fragPos_light_space.xyz = fragPos_light_space.xyz * 0.5 + 0.5;
    float closest_depth = texture(shadow_map[shadow_map_index], fragPos_light_space.xy).r;
    float current_depth = fragPos_light_space.z;
    float bias = 0.005;
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;
    return shadow;
}