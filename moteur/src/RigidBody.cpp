#include <RigidBody.hpp>
#include <SceneNode.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

void RigidBody::updatePhysics(float delta_time) {
    if(use_gravity && !is_on_ground) {
        glm::vec3 gravity = glm::vec3(0.0f,-1.00f,0.0f);
        glm::vec3 acceleration = gravity;
        velocity += acceleration * delta_time;
    }
    applyAirResistance();
    auto shared_node = node.lock();
    if(shared_node) {
        shared_node->transform.adjust_translation(velocity);
        shared_node->transform.transform_updated = true;
    }
    else {
        std::cerr<<"Attempted to update physics on a SceneNode that no longer exists"<<std::endl;
    }
    is_on_ground = false;
}
    

bool RigidBody::checkCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3 &collisionNormal) {
    auto shared_node = node.lock();
    auto other_shared_node = other->node.lock();
    if(shared_node && other_shared_node) {
        if(shared_node->model && other_shared_node->model) {
            return shared_node->model->collider.checkCollision(other_shared_node->model->collider, collisionDepth, collisionNormal);
        }
        else if(shared_node->model && other_shared_node->mesh) {
            return shared_node->model->collider.checkCollision(other_shared_node->mesh->collider, collisionDepth, collisionNormal);
        }
        else if(shared_node->mesh && other_shared_node->model) {
            return shared_node->mesh->collider.checkCollision(other_shared_node->model->collider, collisionDepth, collisionNormal);
        }
        else if(shared_node->mesh && other_shared_node->mesh) {
            return shared_node->mesh->collider.checkCollision(other_shared_node->mesh->collider, collisionDepth, collisionNormal);
        }
    }
    else {
        std::cerr<<"Attempted to update physics on a SceneNode that no longer exists"<<std::endl;
        return false;
    }
}

void RigidBody::solveCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3 &collisionNormal) {
    float safetyOffset = 0.01f;
    collisionNormal = -collisionNormal;
    glm::vec3 correction = (collisionDepth + safetyOffset) * collisionNormal;
    auto shared_node = node.lock();
    if(shared_node) {
        shared_node->transform.adjust_translation(correction);
        shared_node->transform.transform_updated = true;
        if(collisionNormal.y > 0.1f) {
            is_on_ground = true;
            velocity.y = 0;
        }
        //velocity -= glm::dot(velocity, collisionNormal) * collisionNormal; // * restitution_coefficient;
        applyGroundFriction(other);
    }
    else {
        std::cerr<<"Attempted to update physics on a SceneNode that no longer exists"<<std::endl;
    }
}

void RigidBody::applyAirResistance() {
    float air_resistance = 0.05;
    velocity.x *= (1.0f - air_resistance);
    velocity.z *= (1.0f - air_resistance);
}

void RigidBody::applyGroundFriction(std::shared_ptr<RigidBody> other) {
    if(is_on_ground) {
        velocity.x *= (1.0f - other->friction_coefficient);
        velocity.z *= (1.0f - other->friction_coefficient);
    }
}