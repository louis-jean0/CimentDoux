#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <Transform.hpp>

class SceneNode {
    public:
        // Constructor
        SceneNode(Mesh &m);

        // Destructor
        ~SceneNode();

        // Getters
        SceneNode* get_parent();
        Mesh* get_mesh();
        Transform get_transform();

        // Public methods
        void add_child(SceneNode child);
    
    private:
        SceneNode* parent;
        Mesh* mesh;
        Transform transform;
        std::vector<SceneNode*> children;
}