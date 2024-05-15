#include <Player.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

const float CAMERA_HEIGHT = 1.8f;

void Player::update(float delta_time) {
    handleInput(delta_time);
    syncCamera();
    player_node->updateAABB();
    camera->update(delta_time, window);
    //std::cout<<player_node->rigid_body->is_on_ladder<<std::endl;
    //std::cout<<glm::to_string(player_node->get_translation())<<std::endl;
    //std::cout<<player_node->rigid_body->velocity.y<<std::endl;
    //std::cout<<"pos"<<glm::to_string(player_node->get_translation())<<std::endl;
    //std::cout<<"front"<<glm::to_string(camera->getCFront())<<std::endl;
    //std::cout<<player_node->rigid_body->velocity.y<<std::endl;
}

void Player::handleInput(float delta_time) {
    glm::vec3 moveDirection = glm::vec3(0.0f);

    int forward_key = camera->mode_cam == 0 ? GLFW_KEY_Z : GLFW_KEY_W;
    int backward_key = GLFW_KEY_S;
    int left_key = camera->mode_cam == 0 ? GLFW_KEY_Q : GLFW_KEY_A;
    int right_key = GLFW_KEY_D;

    // Handle ladder movement
    if (player_node->rigid_body->is_on_ladder) {
        float ladder_climb_speed = 20.0f;
        player_node->rigid_body->velocity.z = 0.;
        player_node->rigid_body->velocity.x = 0.;
        if (camera->mode_cam == 0 || camera->mode_cam == 1) {
            if (glfwGetKey(window, forward_key) == GLFW_PRESS) { 
                player_node->rigid_body->velocity.y = ladder_climb_speed;
            } else if (glfwGetKey(window, backward_key) == GLFW_PRESS) {
                player_node->rigid_body->velocity.y = -ladder_climb_speed;
            } else {
                player_node->rigid_body->velocity.y = 0.0f;
            }
        }
    } else {
        // Handle horizontal movement
        if (camera->mode_cam == 0 || camera->mode_cam == 1) {
            float sensi = camera->get_sensivity();
            if (glfwGetKey(window, forward_key) == GLFW_PRESS) {
                moveDirection += camera->getCFront() * sensi;
            }
            if (glfwGetKey(window, backward_key) == GLFW_PRESS) {
                moveDirection -= camera->getCFront() * sensi;
            }
            if (glfwGetKey(window, left_key) == GLFW_PRESS) {
                moveDirection += camera->getCRight() * sensi;
            }
            if (glfwGetKey(window, right_key) == GLFW_PRESS) {
                moveDirection -= camera->getCRight() * sensi;
            }
        }

        // Normalize move direction to avoid faster diagonal movement
        if (glm::length(moveDirection) > 0.0f) {
            moveDirection = glm::normalize(moveDirection);
        }

        float max_speed = 12.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            max_speed *= sprint_speed_multiplier; // Adjust max speed when sprinting
        }

        // Apply the movement to the velocity
        float acceleration = 6.0;
        player_node->rigid_body->velocity.x += moveDirection.x * acceleration;
        player_node->rigid_body->velocity.z += moveDirection.z * acceleration;

        glm::vec3 horizontalVelocity = glm::vec3(player_node->rigid_body->velocity.x, 0.0f, player_node->rigid_body->velocity.z);
        if(glm::length(horizontalVelocity) > max_speed) {
            horizontalVelocity = glm::normalize(horizontalVelocity) * max_speed;
            player_node->rigid_body->velocity.x = horizontalVelocity.x;
            player_node->rigid_body->velocity.z = horizontalVelocity.z;
        }
    }

    // Handle jumping
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && player_node->rigid_body->is_on_ground) {
        float jumpHeight = 3.0f;
        float jumpStrength = sqrt(2.0f * 100.0f * jumpHeight); // 100 is the gravity applied to the player rigid body
        player_node->rigid_body->velocity.y = jumpStrength;
        is_jumping = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        is_jumping = false;
    }
}

void Player::syncCamera() {
    glm::vec3 playerPosition = player_node->transform.get_translation();
    camera->setPosition(playerPosition + glm::vec3(0.0f,CAMERA_HEIGHT,0.0f));
}

// void Player::handleSingleInput(int key, int scancode, int action, int mods) {
//     if(action == GLFW_PRESS) {
//         if(key == GLFW_KEY_SPACE) {
//             float jumpStrength = 1.0f;
//             player_node->rigid_body->velocity.y += jumpStrength;
//         }
//     }
// }

std::shared_ptr<Camera> Player::get_camera() {
    return camera;
}

glm::mat4 Player::get_view_matrix() {
    return camera->getViewMatrix();
}

glm::mat4 Player::get_projection_matrix() {
    return camera->getProjectionMatrix();
}

glm::vec3 Player::getCFront() {
    return camera->getCFront();
}

glm::vec3 Player::getCRight() {
    return camera->getCRight();
}

glm::vec3 Player::get_position() {
    return player_node->get_position();
}

void Player::set_vitesse(float vitesse) {
    this->vitesse = vitesse;
}
void Player::set_hauteur(float hauteur) {
    this->hauteur = hauteur;
}
float Player::get_vitesse() {
    this->vitesse = vitesse;
}
float Player::get_hauteur() {
    return this->hauteur;
}
