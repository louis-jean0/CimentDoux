#pragma once

#include <glm/glm.hpp>

class Transform {
    public:
        // Public attributes
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;

        Transform();
        Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

        // Public methods
        glm::mat4 get_matrix();

};