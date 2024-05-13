#include <LightManager.hpp>
#include <memory>

void LightManager::add_directional_light(std::shared_ptr<DirectionalLight> directional_light) {
    this->directional_light = directional_light;
}

void LightManager::add_point_light(std::shared_ptr<PointLight> point_light) {
    point_lights.push_back(point_light);
}

void LightManager::add_torch_light(std::shared_ptr<TorchLight> torch_light) {
    torch_lights.push_back(torch_light);
}

void LightManager::setup_lights(std::shared_ptr<Shader> shader) const {
    shader->useShader();
    int nb_point_lights = point_lights.size();
    int nb_torch_lights = torch_lights.size();
    for(int i = 0; i < nb_point_lights; ++i) {
        point_lights[i]->setup_light(shader, i);
    }
    int shadow_map_index = 10;
    for(int i = 0; i < nb_torch_lights; ++i) {
        torch_lights[i]->setup_light(shader, nb_point_lights + i);
        glActiveTexture(GL_TEXTURE0 + shadow_map_index + i);
        glBindTexture(GL_TEXTURE_2D, torch_lights[i]->fbo_index);
        shader->setBind1i(("shadow_map["+std::to_string(i)+"]").c_str(), shadow_map_index + i);
        shader->setBind1i(("shadow_map_indices["+std::to_string(i)+"]").c_str(), i);
    }
    if(directional_light) directional_light->setup_light(shader, 0);
    shader->setBind1i("nb_point_lights", nb_point_lights + nb_torch_lights);
}