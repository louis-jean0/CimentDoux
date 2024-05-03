#include <RigidBody.hpp>
#include <SceneNode.hpp>
#include <glm/gtx/string_cast.hpp>

void RigidBody::updatePhysics(float delta_time) {
    glm::vec3 gravity = use_gravity ? glm::vec3(0.0f,-9.81f,0.0f) : glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 acceleration = gravity;
    velocity += acceleration * delta_time;
    node->transform.adjust_translation(velocity);
    node->transform.transform_updated = true;
}

bool RigidBody::checkCollision(RigidBody* other, float& collisionDepth, glm::vec3 &collisionNormal) {
    return this->node->model->collider.checkCollision(other->node->model->collider, collisionDepth, collisionNormal);
}

void RigidBody::solveCollision(RigidBody* other, float& collisionDepth, glm::vec3 &collisionNormal) {
    float safetyOffset = 0.01f;
    collisionNormal = -collisionNormal;
    glm::vec3 correction = (collisionDepth + safetyOffset) * collisionNormal;
    node->transform.adjust_translation(correction);
    node->transform.transform_updated = true;
    velocity -= glm::dot(velocity, collisionNormal) * collisionNormal * restitution_coefficient;
    velocity.x *= (1.0f - friction_coefficient);
    velocity.z *= (1.0f - friction_coefficient);
    // if(collisionNormal.y != 1.0f) {
    // std::cout<<"Velocity when collision : "<<glm::to_string(node->rigid_body.velocity)<<std::endl;
    // std::cout<<"Correction : "<<glm::to_string(correction)<<std::endl;
    // std::cout<<"Collision normal : "<<glm::to_string(collisionNormal)<<std::endl;
    // std::cout<<"Depth : "<<collisionDepth<<std::endl;
    // }
}