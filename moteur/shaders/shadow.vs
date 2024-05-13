#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model_matrix;
uniform mat4 shadow_matrices[6];

out vec4 fragPos;

void main() {
    vec4 world_position = model_matrix * vec4(aPos, 1.0);
    fragPos = world_position;
    gl_Position = shadow_matrices[gl_InstanceID] * world_position;
}