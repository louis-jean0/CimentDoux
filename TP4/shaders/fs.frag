#version 460 core

in vec3 normal;
in vec2 uvs;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

out vec4 FragColor;

void main() {
    FragColor = texture(texture_diffuse1,uvs);
}                                                                         
