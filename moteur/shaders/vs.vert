#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 pos;
out vec3 normal;

void main() {
    pos = vec3(model * vec4(aPos, 1.0));
    normal = normalize(mat3(transpose(inverse(model))) * vec3(0.0,0.0,1.0)); // Cheating here
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
