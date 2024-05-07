#include <Player.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

const float MAX_SPEED = 2.0f;
const float ACCELERATION = 0.5f;
const float JUMP_STRENGTH = 1.0f;
const float CAMERA_HEIGHT = 1.8f;

void Player::update(float delta_time) {
    handleInput(delta_time);
    syncCamera();
    player_node->updateAABB();
    camera->update(delta_time, window);
    //std::cout<<glm::to_string(player_node->get_translation())<<std::endl;
    std::cout<<player_node->rigid_body->velocity.y<<std::endl;
    //std::cout<<"pos"<<glm::to_string(player_node->get_translation())<<std::endl;
    //std::cout<<"front"<<glm::to_string(getCFront())<<std::endl;
    //std::cout<<player_node->rigid_body->velocity.y<<std::endl;
}

void Player::handleInput(float delta_time) {
    float fov = camera->getFOV();
    fov -= 10.0f * delta_time;
    if(fov < 90.0f) {
        fov = 90.0f;
    }
    camera->setFOV(fov);
    glm::vec3 moveDirection = glm::vec3(0.0f);

    if(camera->mode_cam==0){
        if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        moveDirection += camera->getCFront();
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            moveDirection -= camera->getCFront();
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            moveDirection += camera->getCRight();
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            moveDirection -= camera->getCRight();
        }
    }
    if(camera->mode_cam==1){
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
    }


    moveDirection.y = 0;
    float acceleration = 0.2f;
    if(glm::length(moveDirection) > 0) {
        moveDirection = glm::normalize(moveDirection);
        player_node->rigid_body->velocity += moveDirection * acceleration * delta_time;
    }
    float max_speed = 1.0f;
    if(glm::length(player_node->rigid_body->velocity) > max_speed) {
        player_node->rigid_body->velocity = glm::normalize(player_node->rigid_body->velocity) * max_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && player_node->rigid_body->is_on_ground) {
        float jumpStrength = sqrt(2.0f * 9.81 * 0.5f);
        float vitesse = 0.01f;
        player_node->rigid_body->velocity.y += jumpStrength * vitesse;
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
