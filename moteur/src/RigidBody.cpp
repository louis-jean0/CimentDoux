// #include <RigidBody.hpp>
// #include <SceneNode.hpp>
// #include <glm/gtx/string_cast.hpp>
// #include <memory>

// void RigidBody::updatePhysics(float delta_time) {
//     use_gravity = !is_on_ladder;
//     if(use_gravity) {
//         glm::vec3 gravity = glm::vec3(0.0f,-100.0f,0.0f);
//         glm::vec3 acceleration = gravity;
//         velocity += acceleration * delta_time;
//         auto shared_node = node.lock();
//         if(shared_node) {
//             shared_node->transform.adjust_translation(velocity * delta_time);
//             shared_node->transform.transform_updated = true;
//         }
//         else {
//         std::cerr<<"Attempted to update physics on a SceneNode that no longer exists"<<std::endl;
//         }
//     }
//     applyAirResistance();
//     is_on_ground = false;
// }

// bool RigidBody::checkCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3 &collisionNormal) {
//     auto shared_node = node.lock();
//     auto other_shared_node = other->node.lock();
//     if(shared_node && other_shared_node) {
//         if(shared_node->model && other_shared_node->model) {
//             return shared_node->model->collider.checkCollision(other_shared_node->model->collider, collisionDepth, collisionNormal);
//         }
//         else if(shared_node->model && other_shared_node->mesh) {
//             return shared_node->model->collider.checkCollision(other_shared_node->mesh->collider, collisionDepth, collisionNormal);
//         }
//         else if(shared_node->mesh && other_shared_node->model) {
//             return shared_node->mesh->collider.checkCollision(other_shared_node->model->collider, collisionDepth, collisionNormal);
//         }
//         else if(shared_node->mesh && other_shared_node->mesh) {
//             return shared_node->mesh->collider.checkCollision(other_shared_node->mesh->collider, collisionDepth, collisionNormal);
//         }
//     }
//     else {
//         std::cerr<<"Attempted to update physics on a SceneNode that no longer exists"<<std::endl;
//         return false;
//     }
//     return false;
// }

// int RigidBody::solveCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3 &collisionNormal) {
//     int acc=-1;
//     float safetyOffset = 0.00001f;
//     glm::vec3 correction = (collisionDepth + safetyOffset) * collisionNormal;
//     auto shared_node = node.lock();
//     if(shared_node) {
//         if(collisionNormal.y > 0.1f) {
//             if(!other->is_trampoline) {
//                 is_on_ground = true;
//             }
//             if(other->restitution_coefficient == 0.0f) {
//                 velocity.y = 0.0f;
//             }
//             else {
//                 float velocity_normal_component = glm::dot(velocity, collisionNormal);
//                 if (velocity_normal_component < 0) {
//                     glm::vec3 velocity_perpendicular = velocity_normal_component * collisionNormal;
//                     glm::vec3 velocity_tangential = velocity - velocity_perpendicular;
//                     velocity = velocity_tangential - other->restitution_coefficient * velocity_perpendicular;
//                 }
//             }
//         }

//         if(other->is_ladder) {
//             use_gravity = false;
//             is_on_ladder = true;
//             acc=0;
//         }

//         if(other->is_in_motion){
//             //std::cout<<"ok pour detection de mouvement"<<std::endl;
//             other->is_child=true;
//             acc=1;
//         }
//         shared_node->transform.adjust_translation(correction);
//         shared_node->transform.transform_updated = true;
//         //velocity -= glm::dot(velocity, collisionNormal) * collisionNormal * other->restitution_coefficient;
//         applyGroundFriction(other);
//     }
//     else {
//         std::cerr<<"Attempted to update physics on a SceneNode that no longer exists"<<std::endl;
//     }
//     return acc;
// }

// void RigidBody::applyAirResistance() {
//     float air_resistance = 0.05f;
//     velocity.x *= (1.0f - air_resistance);
//     velocity.z *= (1.0f - air_resistance);
// }

// void RigidBody::applyGroundFriction(std::shared_ptr<RigidBody> other) {
//     if(is_on_ground) {
//         velocity.x *= (1.0f - other->friction_coefficient);
//         velocity.z *= (1.0f - other->friction_coefficient);
//     }
// }

#include <RigidBody.hpp>
#include <SceneNode.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <iostream>

void RigidBody::updatePhysics(float delta_time) {
    use_gravity = !is_on_ladder;
    if (use_gravity) {
        glm::vec3 gravity = glm::vec3(0.0f, -100.0f, 0.0f);
        glm::vec3 acceleration = gravity;
        velocity += acceleration * delta_time;
    }
    
    applyAirResistance();
    is_on_ground = false;

    auto shared_node = node.lock();
    if (shared_node) {
        shared_node->transform.adjust_translation(velocity * delta_time);
        shared_node->transform.transform_updated = true;
    } else {
        std::cerr << "Attempted to update physics on a SceneNode that no longer exists" << std::endl;
    }
}

bool RigidBody::checkCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3 &collisionNormal) {
    auto shared_node = node.lock();
    auto other_shared_node = other->node.lock();
    if (shared_node && other_shared_node) {
        if (shared_node->model && other_shared_node->model) {
            return shared_node->model->collider.checkCollision(other_shared_node->model->collider, collisionDepth, collisionNormal);
        } else if (shared_node->model && other_shared_node->mesh) {
            return shared_node->model->collider.checkCollision(other_shared_node->mesh->collider, collisionDepth, collisionNormal);
        } else if (shared_node->mesh && other_shared_node->model) {
            return shared_node->mesh->collider.checkCollision(other_shared_node->model->collider, collisionDepth, collisionNormal);
        } else if (shared_node->mesh && other_shared_node->mesh) {
            return shared_node->mesh->collider.checkCollision(other_shared_node->mesh->collider, collisionDepth, collisionNormal);
        }
    } else {
        std::cerr << "Attempted to update physics on a SceneNode that no longer exists" << std::endl;
        return false;
    }
    return false;
}

int RigidBody::solveCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3 &collisionNormal) {
    int acc = -1;
    float safetyOffset = 0.0001f;
    glm::vec3 correction = (collisionDepth + safetyOffset) * collisionNormal;
    auto shared_node = node.lock();
    if (shared_node) {
        if (collisionNormal.y > 0.1f) {
            if (!other->is_trampoline) {
                is_on_ground = true;
            }
            if (other->restitution_coefficient == 0.0f) {
                    velocity.y = 0.0f;
            } else {
                float velocity_normal_component = glm::dot(velocity, collisionNormal);
                if (velocity_normal_component < 0) {
                    glm::vec3 velocity_perpendicular = velocity_normal_component * collisionNormal;
                    glm::vec3 velocity_tangential = velocity - velocity_perpendicular;
                    velocity = velocity_tangential - other->restitution_coefficient * velocity_perpendicular;
                }
            }
        }

        shared_node->transform.adjust_translation(correction);
        shared_node->transform.transform_updated = true;

        if (other->is_ladder) {
            use_gravity = false;
            is_on_ladder = true;
            acc = 0;
        } else {
            use_gravity = true;
            is_on_ladder = false;
        }

        if (other->is_in_motion) {
            other->is_child = true;
            acc = 1;
        }

        //velocity -= glm::dot(velocity, collisionNormal) * collisionNormal * other->restitution_coefficient;
        applyGroundFriction(other);
    } else {
        std::cerr << "Attempted to update physics on a SceneNode that no longer exists" << std::endl;
    }
    return acc;
}

void RigidBody::applyAirResistance() {
    float air_resistance = 0.09f;
    velocity.x *= (1.0f - air_resistance);
    velocity.z *= (1.0f - air_resistance);
}

void RigidBody::applyGroundFriction(std::shared_ptr<RigidBody> other) {
    if (is_on_ground) {
        float friction_force = 1.0f - other->friction_coefficient;
        velocity.x *= friction_force;
        velocity.z *= friction_force;
    }
}