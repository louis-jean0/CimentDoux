#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Model.hpp>
#include <Transform.hpp>
#include <RigidBody.hpp>

class SceneNode {
    public:
        // Public attributes
        std::weak_ptr<SceneNode> parent;
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Model> model;
        Transform transform = Transform();
        RigidBody rigid_body;
        std::vector<std::unique_ptr<SceneNode>> children;

        // Constructor
        SceneNode() : parent(nullptr), mesh(nullptr), model(nullptr), rigid_body(this) {}
        SceneNode(Mesh& mesh) : parent(nullptr), mesh(std::make_unique<Mesh>(mesh)), model(nullptr), rigid_body(this) {}
        SceneNode(Model& model) : parent(nullptr), mesh(nullptr), model(std::make_unique<Model>(model)), rigid_body(this) {}

        // Destructor
        ~SceneNode() = default;

        // Public methods
        void set_parent(std::shared_ptr<SceneNode> parent);
        void add_child(std::unique_ptr<SceneNode> child);
        glm::mat4 get_world_transform();
        void draw(glm::mat4& view, glm::mat4& projection);
        void drawAABB(glm::mat4& view, glm::mat4& projection);
        void enable_physics(bool use_gravity);
        void updateAABB();
};