#include "Transform.hpp"
#include <SceneNode.hpp>
#include <Mesh.hpp

SceneNode::SceneNode(Mesh &mesh) {
    this->mesh = mesh;
}

SceneNode* SceneNode::get_parent() {
    return parent;
}

Mesh* SceneNode::get_mesh() {
    return mesh;
}

Transform SceneNode::get_transform() {
    return transform;
}

void SceneNode::add_child(SceneNode child) {
    this->children.push_back(child);
    child->parent = this;
}

