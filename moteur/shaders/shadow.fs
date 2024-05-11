#version 410 core
in vec4 fragPos;

uniform vec3 light_position;
uniform float far_plane;

void main() {
    float lightDistance = length(fragPos.xyz - light_position);
    lightDistance = lightDistance / far_plane; // Normalisez en fonction de la portée de la lumière
    gl_FragDepth = lightDistance;
}