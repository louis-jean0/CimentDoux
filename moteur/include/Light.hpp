#pragma once

#include <glm/glm.hpp>
#include <Shader.hpp>
#include <memory>

class Light {
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        // Constructors
        Light() : ambient(1.0f), diffuse(1.0f), specular(1.0f) {}
        Light(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular) : ambient(ambient), diffuse(diffuse), specular(specular) {}

        virtual void setup_light(std::shared_ptr<Shader> shader, int light_index) const = 0;
};