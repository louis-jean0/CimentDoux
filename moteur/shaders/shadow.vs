#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 shadow_matrices[6];
uniform float far_plane;
uniform vec3 lightPos;

out vec4 fragPos;

void main() {
    fragPos = vec4(aPos, 1.0);
    gl_Position = shadow_matrices[gl_InstanceID] * vec4(aPos, 1.0);
}