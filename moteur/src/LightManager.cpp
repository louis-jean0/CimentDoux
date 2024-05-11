#include <LightManager.hpp>
#include <memory>

void LightManager::add_light(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

void LightManager::setup_lights(std::shared_ptr<Shader> shader) const {
    shader->useShader();
    int point_light_count = 0;
    int shadow_map_index = 5;
    for (auto& light : lights) {
        if (std::dynamic_pointer_cast<PointLight>(light)) {
            auto point_light = std::dynamic_pointer_cast<PointLight>(light);
            point_light->setup_light(shader, point_light_count++);
            glActiveTexture(GL_TEXTURE0 + shadow_map_index); // 27 but could be whatever else
            glBindTexture(GL_TEXTURE_CUBE_MAP, point_light->fbo_index);
            shader->setBind1i("shadow_map", shadow_map_index);
            shadow_map_index++;

        } else if (std::dynamic_pointer_cast<DirectionalLight>(light)) {
            light->setup_light(shader, 0); // Assumé qu'il n'y a qu'une seule lumière directionnelle
        }
    }
    shader->setBind1i("nb_point_lights", point_light_count);
}