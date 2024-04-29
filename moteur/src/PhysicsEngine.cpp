#include <iostream>
#include <PhysicsEngine.hpp>

void PhysicsEngine::update(float delta_time) {
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->updatePhysics(delta_time);
        for (size_t j = i + 1; j < entities.size(); j++) {
            float collisionDepth;
            glm::vec3 collisionNormal;
            if (entities[i]->checkCollision(entities[j], collisionDepth, collisionNormal)) {
                std::cout << "Collision detected between model at index " << i << " and model at index " << j << std::endl;
                entities[i]->solveCollision(entities[j], collisionDepth, collisionNormal);
            }
        }
    }
}

void PhysicsEngine::add_entity(RigidBody* entity) {
    entities.push_back(entity);
}