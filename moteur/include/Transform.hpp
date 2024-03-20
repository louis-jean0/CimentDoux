#pragma once
#include <glm/glm.hpp>

class Transform {
    public:
        // Public attributes
        glm::mat3 matrix;
        glm::vec3 translation;

        // Public methods
        glm::mat4 apply_to_point();




}