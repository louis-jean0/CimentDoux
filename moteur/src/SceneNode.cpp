#include <glm/gtc/type_ptr.hpp>

#include <Transform.hpp>
#include <SceneNode.hpp>
#include <Mesh.hpp>

// Constructors
SceneNode::SceneNode() {}

SceneNode::SceneNode(Mesh *mesh) {
    this->mesh = mesh;
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
    glm::mat4 parent_transform = parent ? parent->get_world_transform() : glm::mat4(1.0f);
    glm::mat4 local_transform = glm::translate(glm::mat4(1.0f), this->transform.translation) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(this->transform.rotation.x), glm::vec3(1, 0, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(this->transform.rotation.y), glm::vec3(0, 1, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(this->transform.rotation.z), glm::vec3(0, 0, 1));
    
    // Appliquer l'échelle localement, pas dans le calcul hérité
    glm::mat4 scale_transform = glm::scale(glm::mat4(1.0f), this->transform.scale);

    return parent_transform * local_transform * scale_transform;
}


void SceneNode::draw() {
    if(mesh) {
        glm::mat4 model = get_world_transform();
        mesh->shader.useShader();
        mesh->shader.setBindMatrix4fv("model", 1, 0, glm::value_ptr(model));
        mesh->draw();
    }
    for(SceneNode* child : children) {
        child->draw();
    }
}