#pragma once

#include <SceneNode.hpp>
#include <Camera.hpp>
#include <ShaderManager.hpp>
#include <TorchLight.hpp>

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
        p_node->set_scale(glm::vec3(1.0f, 1.0f, 1.0f));
        //spawn debut
        p_node->set_translation(glm::vec3(-8.23f, 10.0f, 21.89f));
        //spawn fin
        //p_node->set_translation(glm::vec3(-20.0f, 100.0f, -17.0f));
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
        //p_node->set_translation(glm::vec3(0.0f));
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
      void set_vitesse(float vitesse);
      void set_hauteur(float hauteur);

      float get_vitesse();
      float get_hauteur();


    private:
		  std::shared_ptr<Camera> camera;
      std::shared_ptr<TorchLight> torch_light;
      GLFWwindow* window; // Lazy to rework this with smart pointers
      bool is_on_ground;
      bool is_jumping;
      float sprint_speed_multiplier = 1.5;
      float user_base_fov = 90.0f;
      float sprint_fov = 110.0f; // Maybe define according to userBaseFOV for a better feeling

      double v0_Vitesse = 0.01f;
      float g = 9.81;
      float hauteur = 1.0f;
      float vitesse = 2.;
      glm::vec3 F = glm::vec3(0., 0., 0.);
      glm::vec3 a = glm::vec3(0., 0., 0.);
      float max_speed = 1.0f;
};