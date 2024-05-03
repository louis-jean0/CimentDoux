#include "RigidBody.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <SceneNode.hpp>
#include <ShaderManager.hpp>
#include <AABB.hpp>

// Constructors
SceneNode::SceneNode() : parent(nullptr), mesh(nullptr), model(nullptr), rigid_body(this) {}

SceneNode::SceneNode(Mesh* mesh) : parent(nullptr), mesh(mesh), model(nullptr), rigid_body(this) {}

SceneNode::SceneNode(Model* model) : parent(nullptr), mesh(nullptr), model(model), rigid_body(this) {}

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

void SceneNode::draw(glm::mat4& view, glm::mat4& projection) {
    glm::mat4 model_matrix = get_world_transform(); // "model_matrix" because "model" not available in this case (confusion with model from SceneNode)
    
    if(model) {
        for(auto& mesh : model->meshes) {
            mesh.shader.useShader();
            mesh.shader.setMVPMatrix(model_matrix, view, projection);
            mesh.draw();
        }
        updateAABB();
        //drawAABB(view, projection);
    }
    else if(mesh) {
        mesh->shader.useShader();
        mesh->shader.setMVPMatrix(model_matrix, view, projection);
        mesh->draw();
    }

    for(SceneNode* child : children) {
        child->draw(view, projection);
    }
}

void SceneNode::updateAABB() {
    if(transform.transform_updated) {
        glm::mat4 model_matrix = get_world_transform();
        model->updateGlobalBoundingBox(model_matrix);
    }
}

void SceneNode::drawAABB(glm::mat4& view, glm::mat4& projection) {
    Shader& aabbShader = ShaderManager::getAABBShader();
    aabbShader.useShader();
    aabbShader.setVPMatrix(view, projection); // No need to send model since we transform the AABB with model when updating
    model->bounding_box.drawBox();
}

void SceneNode::enable_physics(bool use_gravity) {
    rigid_body.use_gravity = use_gravity;
}