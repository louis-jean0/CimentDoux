#pragma once

#include <PointLight.hpp>
#include <ShadowMap.hpp>

class TorchLight : public std::enable_shared_from_this<TorchLight>, Light {
    public:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
        glm::vec3 direction;
        float cut_off;
        float outer_cut_off;
        float near_plane = 0.5f;
        float far_plane = 25.0f;
        bool power = true;
        bool mode = true;
        std::shared_ptr<ShadowMap> shadow_map;
        unsigned int fbo_index;

        TorchLight() : Light() {}
        TorchLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic, glm::vec3& direction, float cut_off, float outer_cut_off) : Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic), direction(direction), cut_off(cut_off), outer_cut_off(outer_cut_off) {}

        static std::shared_ptr<TorchLight> create() {
            return std::make_shared<TorchLight>();
        }

        static std::shared_ptr<TorchLight> create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic, glm::vec3& direction, float cut_off, float outer_cut_off) {
            return std::make_shared<TorchLight>(ambient, diffuse, specular, position, constant, linear, quadratic, direction, cut_off, outer_cut_off);
        }

        void setup_light(std::shared_ptr<Shader> shader, int light_index) const override;
        void gen_shadow_map();
        void setup_shadow_map(std::shared_ptr<Shader> shadow_shader);
        void set_position(glm::vec3 new_pos) {position=new_pos;}
        void off();
        void on();
};