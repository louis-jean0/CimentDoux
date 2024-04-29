#include <iostream>
#include <PhysicsEngine.hpp>

void PhysicsEngine::update(float delta_time) {
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->updatePhysics(delta_time);
        for (size_t j = i + 1; j < entities.size(); j++) { // Commence à j = i + 1 pour éviter les duplications et auto-comparaisons
            if (entities[i]->checkCollision(entities[j])) {
                std::cout << "Collision detected between model at index " << i << " and model at index " << j << std::endl;
                entities[i]->solveCollision(entities[j]);
            }
        }
    }
}

void PhysicsEngine::add_entity(RigidBody* entity) {
    entities.push_back(entity);
}