#include <RigidBody.hpp>
#include <SceneNode.hpp>

void RigidBody::updatePhysics(float delta_time) {
    glm::vec3 gravity = use_gravity ? glm::vec3(0.0f,-1.0f,0.0f) : glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 acceleration = gravity;
    velocity += acceleration * delta_time;
    node->transform.adjust_translation(velocity);
    node->transform.transform_updated = true;
}

bool RigidBody::checkCollision(RigidBody* other, float& collisionDepth, glm::vec3 &collisionNormal) {
    return this->node->model->collider.checkCollision(other->node->model->collider, collisionDepth, collisionNormal);
}

void RigidBody::solveCollision(RigidBody* other, float& collisionDepth, glm::vec3 &collisionNormal) {
    glm::vec3 correction = collisionDepth * (collisionNormal);
    velocity = correction;
}