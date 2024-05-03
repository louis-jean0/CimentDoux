#pragma once

#include <SceneNode.hpp>
#include <Camera.hpp>

class Player {
    public:
      SceneNode* player_node;
      Player(SceneNode* player_node, GLFWwindow* window, Camera* camera) : player_node(player_node), window(window), camera(camera) {
        player_node->enable_physics(true);
        is_jumping = false;
      }
		  void update(float delta_time);
      void handleInput(float delta_time);
      void syncCamera();
      void handleSingleInput(int key, int scancode, int action, int mods);

    private:
		  Camera* camera;
      GLFWwindow* window;
      bool is_on_ground;
      bool is_jumping;
};