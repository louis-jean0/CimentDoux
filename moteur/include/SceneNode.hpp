#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <vector>

#include <Model.hpp>
#include <Transform.hpp>
#include <RigidBody.hpp>

class SceneNode : public std::enable_shared_from_this<SceneNode> {
    public:
        // Public attributes
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Model> model;
        Transform transform = Transform();
        std::shared_ptr<RigidBody> rigid_body;
        
        // Constructor
        SceneNode() = default;
        SceneNode(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}
        SceneNode(std::shared_ptr<Model> model) : model(model) {}

        // Factory
        static std::shared_ptr<SceneNode> create() {
            auto instance = std::make_shared<SceneNode>();
            instance->rigid_body = std::make_shared<RigidBody>(instance->get_shared_ptr());
            return instance;
        }

        static std::shared_ptr<SceneNode> create(std::shared_ptr<Model> model) {
            auto instance = std::make_shared<SceneNode>(model);
            instance->rigid_body = std::make_shared<RigidBody>(instance->get_shared_ptr());
            return instance;
        }

        static std::shared_ptr<SceneNode> create(std::shared_ptr<Mesh> mesh) {
            auto instance = std::make_shared<SceneNode>(mesh);
            instance->rigid_body = std::make_shared<RigidBody>(instance->get_shared_ptr());
            return instance;
        }

        static std::shared_ptr<SceneNode> create(std::shared_ptr<Mesh> mesh, const glm::mat4& transform) {
            auto instance = std::make_shared<SceneNode>(mesh);
            instance->set_transform(transform);
            instance->rigid_body = std::make_shared<RigidBody>(instance->get_shared_ptr());
            return instance;
        }

        static std::vector<std::shared_ptr<SceneNode>> create_node_meshes_from_model(std::shared_ptr<Model> model) {
            std::vector<std::shared_ptr<SceneNode>> instances;
            for(auto& entry : model->entries) {
                auto instance = SceneNode::create(entry.mesh, entry.transform);
                instances.push_back(instance);
            }
            return instances;
        } 

        // Destructor
        ~SceneNode() = default;

        // Public methods
        std::shared_ptr<SceneNode> get_shared_ptr();
        void set_parent(std::shared_ptr<SceneNode> parent);
        void add_child(std::shared_ptr<SceneNode> child);
        glm::mat4 get_world_transform();
        void draw(glm::mat4& view, glm::mat4& projection, int width, int height);
        void computeShadow(std::shared_ptr<Shader> shadow_shader);
        void drawModelAABB(glm::mat4& view, glm::mat4& projection);
        void drawMeshAABB(glm::mat4& view, glm::mat4& projection);
        void enable_physics(bool use_gravity);
        void updateAABB();

        // For transform
        void set_transform(glm::mat4 transform);
        glm::vec3 get_position();
        glm::vec3 get_translation();
        glm::vec3 get_rotation();
        glm::vec3 get_scale();
        void set_translation(glm::vec3 translation);
        void adjust_translation(glm::vec3 delta);
        void set_rotation(glm::vec3 rotation);
        void adjust_rotation(glm::vec3 delta);
        void set_scale(glm::vec3 scale);
        void adjust_scale(glm::vec3 delta);

    private:
        std::weak_ptr<SceneNode> parent;
        std::vector<std::shared_ptr<SceneNode>> children;
};
