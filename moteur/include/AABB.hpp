#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <Shader.hpp>

class AABB {
    public:
        glm::vec3 min, max;
        glm::vec3 originalMin, originalMax;

        AABB();
        AABB(const glm::vec3 vmin, const glm::vec3 vmax);

        void processAABB(const glm::vec3 min, const glm::vec3 max);
        void updateAABB(const glm::mat4& modelMatrix);
        void drawBox();
        glm::vec3 getCenter();
        glm::vec3 getHalfWidths();
};