#include <PhysicsEngine.hpp>
#include <iostream>
#include <climits>

void PhysicsEngine::update(float delta_time) {
    entities[0]->updatePhysics(delta_time); // entities[0] is the player
    for (size_t i = 1; i < entities.size(); ++i) {
        float collisionDepth = FLT_MAX;
        glm::vec3 collisionNormal = glm::vec3(0.0f);
        if (entities[0]->checkCollision(entities[i], collisionDepth, collisionNormal)) {
            //std::cout << "Collision detected between model at index " << 0 << " and model at index " << i << std::endl;
            entities[0]->solveCollision(entities[i], collisionDepth, collisionNormal);
        }
    }
}

void PhysicsEngine::add_player(std::shared_ptr<Player> player) {
    entities.push_back(player->player_node->rigid_body);
}

void PhysicsEngine::add_entity(std::shared_ptr<SceneNode> node) {
    entities.push_back(node->rigid_body);
}