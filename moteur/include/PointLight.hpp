#pragma once

#include <Light.hpp>
#include <vector>

class PointLight : public Light {
    public:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
        bool is_torch_light = false;
        unsigned int shadow_map;
        float far_plane = 25.0f;
        
        // Constructors
        PointLight() : Light() {}
        PointLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

        // Factory
        static std::shared_ptr<PointLight> create() {
            return std::make_shared<PointLight>();
        }

        static std::shared_ptr<PointLight> create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic) {
            return std::make_shared<PointLight>(ambient, diffuse, specular, position, constant, linear, quadratic);
        }

        void setup_light(std::shared_ptr<Shader> shader, int light_index) const override;
        std::vector<glm::mat4> get_shadow_transforms() const;
};