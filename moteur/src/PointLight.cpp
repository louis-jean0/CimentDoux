#include <PointLight.hpp>
#include <string>

void PointLight::setup_shader(Shader& shader, int light_index) const {
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].position").c_str(), position);
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].ambient").c_str(), ambient);
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].diffuse").c_str(), diffuse);
    shader.setVec3(("pointLights["+std::to_string(light_index)+"].specular").c_str(), specular);
    shader.setBind1f(("pointLights["+std::to_string(light_index)+"].constant").c_str(), constant);
    shader.setBind1f(("pointLights["+std::to_string(light_index)+"].linear").c_str(), linear);
    shader.setBind1f(("pointLights["+std::to_string(light_index)+"].quadratic").c_str(), quadratic);
}