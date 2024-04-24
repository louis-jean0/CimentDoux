#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Mesh.hpp>
#include <Transform.hpp>

class SceneNode {
    public:
        // Public attributes
        SceneNode *parent = nullptr;
        Mesh *mesh = nullptr;
        Transform transform;
        std::vector<SceneNode*> children;

        // Constructor
        SceneNode();
        SceneNode(Mesh *mesh);

        // Destructor
        ~SceneNode();

        // Public methods
        void set_parent(SceneNode *parent);
        void add_child(SceneNode *child);
        glm::mat4 get_world_transform();
        void draw();
};