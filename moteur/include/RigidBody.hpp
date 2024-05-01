#pragma once

#include <glm/glm.hpp>

class SceneNode;

class RigidBody {
    public:
        SceneNode* node;
        glm::vec3 velocity;
        float mass;
        bool use_gravity;

        RigidBody(SceneNode* node) : node(node), velocity(0.0f), mass(1.0f), use_gravity(false) {}

        void updatePhysics(float delta_time);
        bool checkCollision(RigidBody* other, float& collisionDepth, glm::vec3& collisionNormal);
        void solveCollision(RigidBody* other, float& collisionDepth, glm::vec3& collisionNormal);
};