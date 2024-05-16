#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aUVs;

// Matrices de transformation
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;
uniform bool hasNormalMap;

out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec2 uvs;
out vec3 fragPos;

void main() {
    normal = normalize(normal_matrix * aNormal);
    if(hasNormalMap) {
        tangent = normalize(normal_matrix * aTangent);
        bitangent = normalize(normal_matrix * aBitangent);
    }
    else {
        tangent = aTangent;
        bitangent = aBitangent;
    }
    uvs = aUVs;
    fragPos = vec3(model * vec4(aPos,1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}