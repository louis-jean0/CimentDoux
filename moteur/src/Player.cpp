#include <Player.hpp>

void Player::update(float delta_time) {
    camera.setPosition(player_node->transform.get_translation());
}