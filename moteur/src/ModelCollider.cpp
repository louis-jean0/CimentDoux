#include <ModelCollider.hpp>
#include <algorithm>
#include <climits>
#include <glm/gtx/string_cast.hpp>

ModelCollider::ModelCollider(AABB& bounding_box) : bounding_box(bounding_box) {}

struct CollisionDepth {
    float depth;
    glm::vec3 axis;
};

CollisionDepth computeCollisionDepth(AABB& a, AABB& b, glm::vec3& axis) {
    float depth = FLT_MAX;
    glm::vec3 a_center = (a.max + a.min) * 0.5f;
    glm::vec3 b_center = (b.max + b.min) * 0.5f;
    float sign = 1.0f;
    if (axis.x != 0.0f) {
        depth = std::min(a.max.x - b.min.x, b.max.x - a.min.x);
        sign = a_center.x > b_center.x ? -1.0f : 1.0f;
    } else if (axis.y != 0.0f) {
        depth = std::min(a.max.y - b.min.y, b.max.y - a.min.y);
        sign = a_center.y > b_center.y ? -1.0f : 1.0f;
    } else if (axis.z != 0.0f) {
        depth = std::min(a.max.z - b.min.z, b.max.z - a.min.z);
        sign = a_center.z > b_center.z ? -1.0f : 1.0f;
    }
    axis *= sign;
    depth = std::max(0.0f, depth);
    return CollisionDepth{depth, axis};
}

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

        CollisionDepth min_depth;
        min_depth.depth = FLT_MAX;
        if (x_depth.depth > 0.0f && x_depth.depth < min_depth.depth) min_depth = x_depth;
        if (y_depth.depth > 0.0f && y_depth.depth < min_depth.depth) min_depth = y_depth;
        if (z_depth.depth > 0.0f && z_depth.depth < min_depth.depth) min_depth = z_depth;
        if (min_depth.depth < FLT_MAX) {
            collisionNormal = min_depth.axis;
            collisionDepth = min_depth.depth;
            return true;
        }
    }
    return false;
}