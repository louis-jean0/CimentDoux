#include <Player.hpp>

void Player::update(float delta_time) {
    handleInput(delta_time);
    syncCamera();
    player_node->updateAABB();
    camera->update(delta_time, window);
}

void Player::handleInput(float delta_time) {
    float fov = camera->getFOV();
    fov -= 10.0f * delta_time;
    if(fov < 45.0f) {
        fov = 45.0f;
    }
    camera->setFOV(fov);
    glm::vec3 moveDirection = glm::vec3(0.0f);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        moveDirection += camera->getCFront();
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        moveDirection -= camera->getCFront();
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        moveDirection += camera->getCRight();
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        moveDirection -= camera->getCRight();
    }
    moveDirection.y = 0;
    float acceleration = 0.1f;
    player_node->rigid_body.velocity += moveDirection * acceleration * delta_time;
    float max_speed = 1.0f;
    if(glm::length(player_node->rigid_body.velocity) > max_speed) {
        player_node->rigid_body.velocity = glm::normalize(player_node->rigid_body.velocity) * max_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        float jumpStrength = 0.1f;
        player_node->rigid_body.velocity.y += jumpStrength;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        fov += 50.0f * delta_time;
        if(fov > 90.0f) fov = 90.0f;
        camera->setFOV(fov);
    }
    
    //std::cout<<player_node->rigid_body.velocity.y<<std::endl;
}

void Player::syncCamera() {
    glm::vec3 playerPosition = player_node->transform.get_translation();
    camera->setPosition(playerPosition + glm::vec3(0.0f,3.0f,0.0f));
}

void Player::handleSingleInput(int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_SPACE) {
            float jumpStrength = 1.0f;
            player_node->rigid_body.velocity.y += jumpStrength;
        }
    }
}