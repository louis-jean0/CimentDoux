#include <TorchLight.hpp>

void TorchLight::setup_light(std::shared_ptr<Shader> shader, int light_index) const {
    PointLight::setup_light(shader, light_index);
    glm::mat4 light_projection = glm::perspective(glm::radians(cut_off), float(shadow_map->SHADOW_WIDTH) / float(shadow_map->SHADOW_HEIGHT), 1.0f, 25.0f);
    glm::mat4 light_view = glm::lookAt(position, position + direction, glm::vec3(0.0, 1.0, 0.0));
    shader->setVec3(("pointLights[" + std::to_string(light_index) + "].direction").c_str(), direction);
    shader->setBind1f(("pointLights[" + std::to_string(light_index) + "].cut_off").c_str(), glm::cos(glm::radians(cut_off)));
    shader->setBind1f(("pointLights[" + std::to_string(light_index) + "].outer_cut_off").c_str(), glm::cos(glm::radians(outer_cut_off)));
    shader->setBindMatrix4fv("light_projection", 1, 0, glm::value_ptr(light_projection));
    shader->setBindMatrix4fv("light_view", 1, 0, glm::value_ptr(light_view));
}

void TorchLight::gen_shadow_map() {
    shadow_map = ShadowMap::create(shared_from_this());
}

void TorchLight::setup_shadow_map(std::shared_ptr<Shader> shadow_shader) {
    glm::mat4 light_projection = glm::perspective(glm::radians(cut_off), float(shadow_map->SHADOW_WIDTH) / float(shadow_map->SHADOW_HEIGHT), 1.0f, 25.0f);
    glm::mat4 light_view = glm::lookAt(position, position + direction, glm::vec3(0.0, 1.0, 0.0));
    shadow_shader->setBindMatrix4fv("light_projection", 1, 0, glm::value_ptr(light_projection));
    shadow_shader->setBindMatrix4fv("light_view", 1, 0, glm::value_ptr(light_view));
}