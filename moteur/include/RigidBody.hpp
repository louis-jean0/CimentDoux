#pragma once

#include <glm/glm.hpp>

class SceneNode;

class RigidBody {
    public:
        SceneNode* node;
        glm::vec3 velocity;
        float mass;
        float friction_coefficient;
        float restitution_coefficient;
        bool use_gravity;

        RigidBody(SceneNode* node) : node(node), velocity(0.0f), mass(1.0f), use_gravity(false), friction_coefficient(1.0f), restitution_coefficient(0.0f) {}

        void updatePhysics(float delta_time);
        bool checkCollision(RigidBody* other, float& collisionDepth, glm::vec3& collisionNormal);
        void solveCollision(RigidBody* other, float& collisionDepth, glm::vec3& collisionNormal);
};