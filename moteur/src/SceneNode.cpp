#include <glm/gtc/type_ptr.hpp>

#include <Transform.hpp>
#include <SceneNode.hpp>
#include <Mesh.hpp>

// Constructors
SceneNode::SceneNode() {}

SceneNode::SceneNode(Mesh *mesh) {
    this->mesh = mesh;
}

SceneNode::SceneNode(Model *model) {
    this->model = model;
}

// Destructor
SceneNode::~SceneNode() {
    for(SceneNode* child : children) {
        delete child;
    }
}

// Public methods
void SceneNode::set_parent(SceneNode *parent) {
    this->parent = parent;
    parent->children.push_back(this);
}

void SceneNode::add_child(SceneNode *child) {
    child->set_parent(this);
}

glm::mat4 SceneNode::get_world_transform() {
    if(parent) {
        return parent->get_world_transform() * transform.get_matrix();
    }
    else {
        return transform.get_matrix();
    }
}

void SceneNode::draw() {
    glm::mat4 model_matrix = get_world_transform();
    if(model) {
        if(transform.transform_updated) {
            model->updateGlobalBoundingBox(model_matrix);
            transform.transform_updated = false;
            model->bounding_box.drawBox();
        }
        for(auto& mesh : model->meshes) {
            mesh.shader.useShader();
            mesh.shader.setBindMatrix4fv("model", 1, 0, glm::value_ptr(model_matrix));
            mesh.draw();
        }
    }
    else if(mesh) {
        mesh->shader.useShader();
        mesh->shader.setBindMatrix4fv("model", 1, 0, glm::value_ptr(model_matrix));
        mesh->draw();
    }
    for(SceneNode* child : children) {
        child->draw();
    }
}