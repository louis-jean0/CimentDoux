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

    /*
    moveDirection.y = 0;
    float acceleration = 0.1f;
    player_node->rigid_body.velocity += moveDirection * acceleration * delta_time;
    float max_speed = 1.0f;
    if(glm::length(player_node->rigid_body.velocity) > max_speed) {
        player_node->rigid_body.velocity = glm::normalize(player_node->rigid_body.velocity) * max_speed;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !Space) {
        v0_Vitesse = sqrt(2.0f * g * hauteur);
        Space = true;
        player_node->rigid_body.velocity += v0_Vitesse;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        Space = false;
    }*/


    if(glm::length(glm::vec2(player_node->rigid_body.velocity.x, player_node->rigid_body.velocity.z)) > max_speed) {
        glm::vec2 horizontal_velocity = glm::normalize(glm::vec2(player_node->rigid_body.velocity.x, player_node->rigid_body.velocity.z)) * max_speed;
        player_node->rigid_body.velocity.x = horizontal_velocity.x;
        player_node->rigid_body.velocity.z = horizontal_velocity.y;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !Space) {
        // Dérivée de la conservation de l'énergie
        player_node->rigid_body.velocity.y = sqrt(2.0f * g * hauteur);
        Space = true;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        Space = false;
    }

    if(player_node->transform.get_translation().y <= 0.0f) {
        player_node->transform.set_translation(glm::vec3(
            player_node->transform.get_translation().x,
            1.0f,
            player_node->transform.get_translation().z
        ));
        player_node->rigid_body.velocity.y = 0.0f;     
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