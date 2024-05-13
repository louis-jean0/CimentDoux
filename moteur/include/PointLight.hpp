#pragma once

#include <Light.hpp>
#include <memory>
#include <vector>
#include <ShadowMap.hpp>

class PointLight : public std::enable_shared_from_this<PointLight>, Light {
    public:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
        bool is_torch_light = false;
        std::shared_ptr<ShadowMap> shadow_map;
        unsigned int fbo_index;
        float far_plane = 25.0f;
        
        // Constructors
        PointLight() : Light() {}
        PointLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic) {}
        ~PointLight() {}

        // Factory
        static std::shared_ptr<PointLight> create() {
            return std::make_shared<PointLight>();
        }

        static std::shared_ptr<PointLight> create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic) {
            return std::make_shared<PointLight>(ambient, diffuse, specular, position, constant, linear, quadratic);
        }

        void setup_light(std::shared_ptr<Shader> shader, int light_index) const override;
        void setup_shadow_map(std::shared_ptr<Shader> shadow_shader) const;
        void gen_shadow_map();
        std::vector<glm::mat4> get_shadow_transforms() const;
};