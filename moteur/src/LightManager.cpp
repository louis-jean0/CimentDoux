#include <LightManager.hpp>

void LightManager::add_light(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

void LightManager::setup_lights(std::shared_ptr<Shader> shader) const {
    shader->setBind1i("nb_point_lights", lights.size());
    for(int i = 0; i < lights.size(); ++i) {
        lights[i]->setup_light(shader, i);
    }
}