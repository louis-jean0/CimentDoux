#version 330 core
  
in vec3 pos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = ambientStrength * vec3(1.0);

    // Diffuse 
    vec3 lightDir = normalize(lightPos - pos);
    float diff = max(dot(normal, lightDir), 0.0); // normal has been normalized in the vertex shader
    vec3 diffuse = diff * vec3(diffuseStrength);

    // Specular    
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * vec3(1.0);
            
    vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
} 