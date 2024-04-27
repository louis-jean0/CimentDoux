#include <ModelCollider.hpp>

ModelCollider::ModelCollider(const AABB& bounding_box) {
    this->bounding_box = bounding_box;
}

bool ModelCollider::checkCollision(const ModelCollider& other) const {
    return (bounding_box.max.x >= other.bounding_box.min.x && bounding_box.min.x <= other.bounding_box.max.x) &&
           (bounding_box.max.y >= other.bounding_box.min.y && bounding_box.min.y <= other.bounding_box.max.y) &&
           (bounding_box.max.z >= other.bounding_box.min.z && bounding_box.min.z <= other.bounding_box.max.z);
}