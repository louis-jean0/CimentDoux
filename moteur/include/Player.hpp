#pragma once

#include <SceneNode.hpp>
#include <Camera.hpp>
#include <ShaderManager.hpp>

class Player {
    public:
      std::shared_ptr<SceneNode> player_node;

      // Constructor
      Player(std::shared_ptr<SceneNode> player_node, GLFWwindow* window, Camera* camera) : player_node(player_node), window(window), camera(camera) {
        player_node->enable_physics(true);
        is_jumping = false;
      }

      Player(GLFWwindow* window) : window(window) {
        auto p_model = Model::create("../data/models/capsule/capsule.gltf", ShaderManager::getShader());
        auto p_node = SceneNode::create(p_model);
        p_node->set_scale(glm::vec3(1.0f));
        p_node->set_translation(glm::vec3(-20.0f, 50.0f, 20.0f));
        p_node->set_rotation(glm::vec3(0.0f,0.0f,90.0f));
        p_node->enable_physics(true);
        this->player_node = p_node;
        this->camera = std::make_shared<Camera>();
        camera->init();
        is_jumping = false;
      }

      Player(GLFWwindow* window, Camera* camera) : window(window), camera(camera) {
        auto p_model = Model::create("../data/models/capsule/capsule.gltf", ShaderManager::getShader());
        auto p_node = SceneNode::create(p_model);
        p_node->set_scale(glm::vec3(1.0f));
        p_node->set_translation(glm::vec3(-20.0f));
        p_node->set_rotation(glm::vec3(0.0f,0.0f,90.0f));
        p_node->enable_physics(true);
        this->player_node = p_node;
        is_jumping = false;
      }

      // Factory
      static std::shared_ptr<Player> create(std::shared_ptr<SceneNode> player_node, GLFWwindow* window, Camera* camera) {
        return std::make_shared<Player>(player_node, window, camera);
      }

      static std::shared_ptr<Player> create(GLFWwindow* window) {
        return std::make_shared<Player>(window);
      }

      static std::shared_ptr<Player> create(GLFWwindow* window, Camera* camera) {
        return std::make_shared<Player>(window, camera);
      }
      
		  void update(float delta_time);
      void handleInput(float delta_time);
      void syncCamera();
      void handleSingleInput(int key, int scancode, int action, int mods);
      std::shared_ptr<Camera> get_camera();
      glm::mat4 get_view_matrix();
      glm::mat4 get_projection_matrix();
      glm::vec3 getCFront();
      glm::vec3 getCRight();
      glm::vec3 get_position();

    private:
		  std::shared_ptr<Camera> camera;
      GLFWwindow* window; // Lazy to rework this with smart pointers
      bool is_on_ground;
      bool is_jumping;
};