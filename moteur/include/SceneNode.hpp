#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Model.hpp>
#include <Transform.hpp>

class SceneNode {
    public:
        // Public attributes
        SceneNode *parent = nullptr;
        Mesh *mesh = nullptr;
        Model *model = nullptr;
        Transform transform = Transform();
        std::vector<SceneNode*> children;

        // Constructor
        SceneNode();
        SceneNode(Mesh* mesh);
        SceneNode(Model* model);

        // Destructor
        ~SceneNode();

        // Public methods
        void set_parent(SceneNode *parent);
        void add_child(SceneNode *child);
        glm::mat4 get_world_transform();
        void draw(glm::mat4& view, glm::mat4& projection);
};