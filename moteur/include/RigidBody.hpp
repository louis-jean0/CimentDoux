#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class SceneNode;

class RigidBody {
    public:
        std::weak_ptr<SceneNode> node;
        std::vector<std::weak_ptr<RigidBody>> active_collisions;
        glm::vec3 velocity;
        float mass;
        float friction_coefficient;
        float restitution_coefficient;
        bool use_gravity;
        bool is_on_ground;
        bool is_ladder = false;
        bool is_on_ladder = false;
        bool is_trampoline = false;
        bool is_in_motion = false;
        bool is_child = false;

        RigidBody() = default;
        RigidBody(std::weak_ptr<SceneNode> node) : node(node), velocity(0.0f), mass(1.0f), use_gravity(false), is_on_ground(false), friction_coefficient(0.05f), restitution_coefficient(0.0f) {}
        ~RigidBody() = default;

        void updatePhysics(float delta_time);
        bool checkCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3& collisionNormal);
        int solveCollision(std::shared_ptr<RigidBody> other, float& collisionDepth, glm::vec3& collisionNormal);
        void applyAirResistance();
        void applyGroundFriction();
        void applyGroundFriction(std::shared_ptr<RigidBody> other);
        void startCollision(std::shared_ptr<RigidBody> other);
        void endCollision(std::shared_ptr<RigidBody> other);
};