#pragma once

#include <Light.hpp>
#include <memory>

class DirectionalLight : public Light {
    public:
        glm::vec3 direction;

        // Constructor
        DirectionalLight() : Light() {}
        DirectionalLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& direction) : Light(ambient, diffuse, specular), direction(direction) {}

        // Factory
        static std::shared_ptr<DirectionalLight> create() {
            return std::make_shared<DirectionalLight>();
        }

        static std::shared_ptr<DirectionalLight> create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& direction) {
            return std::make_shared<DirectionalLight>(ambient, diffuse, specular, direction);
        }

        void setup_light(std::shared_ptr<Shader> shader, int light_index) const override;
};