#include <TorchLight.hpp>

void TorchLight::setup_light(std::shared_ptr<Shader> shader, int light_index) const {
    shader->setVec3(("torchLights["+std::to_string(light_index)+"].position").c_str(), position);
    shader->setVec3(("torchLights["+std::to_string(light_index)+"].ambient").c_str(), ambient);
    shader->setVec3(("torchLights["+std::to_string(light_index)+"].diffuse").c_str(), diffuse);
    shader->setVec3(("torchLights["+std::to_string(light_index)+"].specular").c_str(), specular);
    shader->setBind1f(("torchLights["+std::to_string(light_index)+"].constant").c_str(), constant);
    shader->setBind1f(("torchLights["+std::to_string(light_index)+"].linear").c_str(), linear);
    shader->setBind1f(("torchLights["+std::to_string(light_index)+"].quadratic").c_str(), quadratic);
    shader->setVec3(("torchLights[" + std::to_string(light_index) + "].direction").c_str(), direction);
    shader->setBind1f(("torchLights[" + std::to_string(light_index) + "].cut_off").c_str(), glm::cos(glm::radians(cut_off)));
    shader->setBind1f(("torchLights[" + std::to_string(light_index) + "].outer_cut_off").c_str(), glm::cos(glm::radians(outer_cut_off)));
    shader->setBind1f(("torchLights[" + std::to_string(light_index) + "].far_plane").c_str(), far_plane);
    glm::mat4 light_projection = glm::perspective(glm::radians(std::max(cut_off * 2.0f,outer_cut_off)), float(shadow_map->SHADOW_WIDTH) / float(shadow_map->SHADOW_HEIGHT), near_plane, far_plane);
    glm::mat4 light_view = glm::lookAt(position, position + direction, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 light_space_matrix = light_projection * light_view;
    shader->setBindMatrix4fv(("torchLights["+std::to_string(light_index)+"].light_space_matrix").c_str(), 1, 0, glm::value_ptr(light_space_matrix));
}

void TorchLight::gen_shadow_map() {
    shadow_map = ShadowMap::create(shared_from_this());
}

void TorchLight::setup_shadow_map(std::shared_ptr<Shader> shadow_shader) {
    glm::mat4 light_projection = glm::perspective(glm::radians(std::max(cut_off * 2.0f,outer_cut_off)), float(shadow_map->SHADOW_WIDTH) / float(shadow_map->SHADOW_HEIGHT), near_plane, far_plane);
    glm::mat4 light_view = glm::lookAt(position, position + direction, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 light_space_matrix = light_projection * light_view;
    shadow_shader->setBindMatrix4fv("light_space_matrix", 1, 0, glm::value_ptr(light_space_matrix));
}

void TorchLight::off(){
    constant=5.f;
    linear=5.f;
    quadratic=5.f;
}
void TorchLight::on(){
    if(mode){
        constant=1.f; 
        linear=0.3f; 
        quadratic=0.002f;
    }else{
        constant=1.f; 
        linear=0.003f; 
        quadratic=0.002f;
    }
}