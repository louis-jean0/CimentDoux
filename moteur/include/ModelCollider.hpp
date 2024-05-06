#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <AABB.hpp>

class ModelCollider {
    public:
        AABB& bounding_box;
        ModelCollider(AABB& bounding_box);
        bool checkCollision(ModelCollider& other, float& collisionDepth, glm::vec3& collisionNormal);
};