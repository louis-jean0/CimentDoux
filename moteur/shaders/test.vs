#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUVs;

// Matrices de transformation
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 uvs;
out vec3 fragPos;

void main() {
    normal = normalize(transpose(inverse(mat3(model))) * aNormal);
    uvs = aUVs;
    fragPos = vec3(model * vec4(aPos,1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}