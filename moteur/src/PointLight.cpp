#include <PointLight.hpp>
#include <ShaderManager.hpp>
#include <memory>

void PointLight::setup_light(std::shared_ptr<Shader> shader, int light_index) const {
    shader->setVec3(("pointLights["+std::to_string(light_index)+"].position").c_str(), position);
    shader->setVec3(("pointLights["+std::to_string(light_index)+"].ambient").c_str(), ambient);
    shader->setVec3(("pointLights["+std::to_string(light_index)+"].diffuse").c_str(), diffuse);
    shader->setVec3(("pointLights["+std::to_string(light_index)+"].specular").c_str(), specular);
    shader->setBind1f(("pointLights["+std::to_string(light_index)+"].constant").c_str(), constant);
    shader->setBind1f(("pointLights["+std::to_string(light_index)+"].linear").c_str(), linear);
    shader->setBind1f(("pointLights["+std::to_string(light_index)+"].quadratic").c_str(), quadratic);
    shader->setBool(("pointLights["+std::to_string(light_index)+"].is_torch_light").c_str(), is_torch_light);
    shader->setBind1f(("pointLights["+std::to_string(light_index)+"].far_plane").c_str(), far_plane);
}

void PointLight::setup_shadow_map(std::shared_ptr<Shader> shadow_shader) const {
    shadow_shader->setBind1f("far_plane", far_plane);
    shadow_shader->setVec3("light_position", position);
    std::vector<glm::mat4> shadow_transforms = get_shadow_transforms();
    for(int i = 0; i < 6; ++i) {
        shadow_shader->setBindMatrix4fv(("shadow_matrices["+std::to_string(i)+"]").c_str(), 1, 0, glm::value_ptr(shadow_transforms[i]));
    }
}

std::vector<glm::mat4> PointLight::get_shadow_transforms() const {
    float near_plane = 1.0f;
    float aspect = (float)shadow_map->SHADOW_WIDTH/shadow_map->SHADOW_HEIGHT;
    glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);
    std::vector<glm::mat4> shadow_transforms;
    shadow_transforms.push_back(shadow_proj * glm::lookAt(position, position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
    shadow_transforms.push_back(shadow_proj * glm::lookAt(position, position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
    shadow_transforms.push_back(shadow_proj * glm::lookAt(position, position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
    shadow_transforms.push_back(shadow_proj * glm::lookAt(position, position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
    shadow_transforms.push_back(shadow_proj * glm::lookAt(position, position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
    shadow_transforms.push_back(shadow_proj * glm::lookAt(position, position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));
    return shadow_transforms;
}

void PointLight::gen_shadow_map() {
    shadow_map = ShadowMap::create(shared_from_this());
}