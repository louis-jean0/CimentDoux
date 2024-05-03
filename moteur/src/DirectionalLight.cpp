#include <DirectionalLight.hpp>
#include <string>

void DirectionalLight::setup_shader(Shader& shader, int light_index) const {
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].direction").c_str(), direction);
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].ambient").c_str(), ambient);
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].diffuse").c_str(), diffuse);
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].specular").c_str(), specular);
}