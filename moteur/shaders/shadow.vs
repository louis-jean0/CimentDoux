#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model_matrix;
uniform mat4 light_projection;
uniform mat4 light_view;

void main() {

    gl_Position = light_projection * light_view * model_matrix * vec4(aPos, 1.0);
}