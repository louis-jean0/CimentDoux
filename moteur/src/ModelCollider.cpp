#include <ModelCollider.hpp>
#include <algorithm>

struct CollisionDepth {
    float depth;
    glm::vec3 axis;
};

CollisionDepth computeCollisionDepth(AABB& a, AABB& b, glm::vec3& axis) {
    float depth = 0.0f;
    if(axis == glm::vec3(1.0f,0.0f,0.0f)) {
        depth = std::min(a.max.x - b.min.x, b.max.x - a.min.x);
    }
    else if(axis == glm::vec3(0.0f,1.0f,0.0f)) {
        depth = std::min(a.max.y - b.min.y, b.max.y - a.min.y);
    }
    else if(axis == glm::vec3(0.0f,0.0f,1.0f)) {
        depth = std::min(a.max.z - b.min.z, b.max.z - a.min.z);
    }
    depth = std::max(depth, 0.0f);
    return CollisionDepth{depth, axis};
}

ModelCollider::ModelCollider(AABB& bounding_box) : bounding_box(bounding_box) {}

bool ModelCollider::checkCollision(ModelCollider& other, float& collisionDepth, glm::vec3& collisionNormal) {
    if ((bounding_box.max.x >= other.bounding_box.min.x && bounding_box.min.x <= other.bounding_box.max.x) &&
           (bounding_box.max.y >= other.bounding_box.min.y && bounding_box.min.y <= other.bounding_box.max.y) &&
           (bounding_box.max.z >= other.bounding_box.min.z && bounding_box.min.z <= other.bounding_box.max.z)) {

        glm::vec3 x_axis = glm::vec3(1.0f,0.0f,0.0f);
        glm::vec3 y_axis = glm::vec3(0.0f,1.0f,0.0f);
        glm::vec3 z_axis = glm::vec3(0.0f,0.0f,1.0f);
        CollisionDepth x_depth = computeCollisionDepth(bounding_box, other.bounding_box, x_axis);
        CollisionDepth y_depth = computeCollisionDepth(bounding_box, other.bounding_box, y_axis);
        CollisionDepth z_depth = computeCollisionDepth(bounding_box, other.bounding_box, z_axis);

        CollisionDepth min_depth = x_depth;
        if(y_depth.depth < min_depth.depth) min_depth = y_depth;
        if(z_depth.depth < min_depth.depth) min_depth = z_depth;
        collisionNormal = min_depth.axis;
        return true;
    }
    return false;
}