#include <LightManager.hpp>

void LightManager::add_light(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

void LightManager::setup_lights(std::shared_ptr<Shader> shader) const {
    int point_light_count = 0;
    for (auto& light : lights) {
        if (std::dynamic_pointer_cast<PointLight>(light)) {
            light->setup_light(shader, point_light_count++);
        } else if (std::dynamic_pointer_cast<DirectionalLight>(light)) {
            light->setup_light(shader, 0); // Assumé qu'il n'y a qu'une seule lumière directionnelle
        }
    }
    shader->setBind1i("nb_point_lights", point_light_count);
}