#include <TorchLight.hpp>

void TorchLight::setup_light(std::shared_ptr<Shader> shader, int light_index) const {
    PointLight::setup_light(shader, light_index);
    shader->setVec3(("pointLights[" + std::to_string(light_index) + "].direction").c_str(), direction);
    shader->setBind1f(("pointLights[" + std::to_string(light_index) + "].cut_off").c_str(), glm::cos(glm::radians(cut_off)));
    shader->setBind1f(("pointLights[" + std::to_string(light_index) + "].outer_cut_off").c_str(), glm::cos(glm::radians(outer_cut_off)));
}

std::vector<glm::mat4> TorchLight::get_shadow_transforms() const {
    PointLight::get_shadow_transforms();
}