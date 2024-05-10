#pragma once

#include <PointLight.hpp>

class TorchLight : public PointLight {
    public:
        glm::vec3 direction;
        float cut_off;
        float outer_cut_off;

        TorchLight() : PointLight() {
            is_torch_light = true;
        }
        TorchLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic, glm::vec3& direction, float cut_off, float outer_cut_off) : PointLight(ambient, diffuse, specular, position, constant, linear, quadratic), direction(direction), cut_off(cut_off), outer_cut_off(outer_cut_off) {
            is_torch_light = true;
        }

        static std::shared_ptr<TorchLight> create() {
            return std::make_shared<TorchLight>();
        }

        static std::shared_ptr<TorchLight> create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic, glm::vec3& direction, float cut_off, float outer_cut_off) {
            return std::make_shared<TorchLight>(ambient, diffuse, specular, position, constant, linear, quadratic, direction, cut_off, outer_cut_off);
        }

        void setup_light(std::shared_ptr<Shader> shader, int light_index) const override;
        std::vector<glm::mat4> get_shadow_transforms() const;

};