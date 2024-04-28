#version 460 core

out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform float cutOff;
uniform float outerCutOff;
uniform vec3 lightambient;
uniform vec3 lightdiffuse;
uniform vec3 lightspecular;
uniform float constant;
uniform float linear;
uniform float quadratic;


uniform sampler2D texture_diffuse1;

void main()
{
    vec2 st = gl_FragCoord.xy / vec2(1280, 720);

    // Ambient
    vec3 ambient = lightambient * texture(texture_diffuse1, TexCoords).rgb;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDire = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDire), 0.0);
    vec3 diffuse = lightdiffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDire, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = lightspecular * spec * 0.5;

    // Spotlight
    float theta = dot(lightDire, normalize(-lightDir));  // Utilisez la direction de la lumière
    float epsilon = (cutOff - outerCutOff);
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // Attenuation
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
