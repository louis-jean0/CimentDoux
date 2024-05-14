#pragma once

#include <SceneNode.hpp>
#include <LightManager.hpp>
#include <ShaderManager.hpp>
#include <PhysicsEngine.hpp>
#include <memory>

class Scene {
    public:
        std::vector<std::shared_ptr<SceneNode>> scene_nodes;
        Scene() {}
        Scene(std::shared_ptr<Player> player ) : player(player) {}

        // Factory
        static std::shared_ptr<Scene> create() {
            auto instance = std::make_shared<Scene>();
            instance->setup_scene();
            return instance;
        }

        static std::shared_ptr<Scene> create(std::shared_ptr<Player> player) {
            auto instance = std::make_shared<Scene>(player);
            instance->setup_scene();
            return instance;
        }

        void setup_scene();
        void add_node(std::shared_ptr<SceneNode> node);
        void add_model(std::shared_ptr<Model> model);
        void add_meshes_from_model(std::shared_ptr<Model> model);
        void add_entities_into_physics_engine(std::shared_ptr<PhysicsEngine> pe);
        void draw(glm::mat4& view, glm::mat4& projection);
        void update_light_player();
    
    private:
        std::shared_ptr<LightManager> lights = std::make_shared<LightManager>();
        std::shared_ptr<ShaderManager> shaders = std::make_shared<ShaderManager>();
        std::shared_ptr<Player> player ;
};