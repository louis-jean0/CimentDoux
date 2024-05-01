#include <PhysicsEngine.hpp>
#include <iostream>

void PhysicsEngine::update(float delta_time) {
    entities[0]->updatePhysics(delta_time); // entities[0] is the player
    for (size_t i = 1; i < entities.size(); ++i) {
        float collisionDepth;
        glm::vec3 collisionNormal;
        if (entities[0]->checkCollision(entities[i], collisionDepth, collisionNormal)) {
            std::cout << "Collision detected between model at index " << 0 << " and model at index " << i << std::endl;
            entities[0]->solveCollision(entities[i], collisionDepth, collisionNormal);
        }
    }
}

void PhysicsEngine::add_entity(RigidBody *entity) {
  entities.push_back(entity);
}