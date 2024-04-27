#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <AABB.hpp>

class ModelCollider {
    public:
        ModelCollider(const AABB& bounding_box);
        bool checkCollision(const ModelCollider& other) const;

    private:
        AABB bounding_box;
};