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
      bool Space = false;

      double v0_Vitesse = 0.01f;
      float g = 9.81;
      float hauteur = 1.0f;
      float vitesse = 2.;
      glm::vec3 F = glm::vec3(0., 0., 0.);
      glm::vec3 a = glm::vec3(0., 0., 0.);
      float max_speed = 1.0f;
};