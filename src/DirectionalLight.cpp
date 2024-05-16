#include <DirectionalLight.hpp>
#include <string>

void DirectionalLight::setup_light(std::shared_ptr<Shader> shader, int light_index) const {
    shader->setVec3("directionalLight.direction", direction);
    shader->setVec3("directionalLight.ambient", ambient);
    shader->setVec3("directionalLight.diffuse", diffuse);
    shader->setVec3("directionalLight.specular", specular);
}