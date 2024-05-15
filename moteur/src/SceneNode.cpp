#include <SceneNode.hpp>
#include <ShaderManager.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

// Public methods
std::shared_ptr<SceneNode> SceneNode::get_shared_ptr() {
    return shared_from_this();
}

void SceneNode::set_parent(std::shared_ptr<SceneNode> parent) {
    this->parent = parent;
}

void SceneNode::add_child(std::shared_ptr<SceneNode> child) {
    children.push_back(child);
    child->set_parent(shared_from_this());
}

glm::mat4 SceneNode::get_world_transform() {
    if (auto parent_ptr = parent.lock()) {
        return parent_ptr->get_world_transform() * transform.get_matrix();
    } else {
        return transform.get_matrix();
    }
}

void SceneNode::draw(glm::mat4& view, glm::mat4& projection, int width, int height) {
    glm::mat4 model_matrix = get_world_transform();

    if (model) {
        updateAABB();
        //drawModelAABB(view, projection);
        for (const auto entry : model->entries) {
            entry.mesh->shader->useShader();
            entry.mesh->shader->setMVPMatrix(model_matrix, view, projection);
            glm::mat3 normal_matrix = glm::inverse(glm::transpose(model_matrix));
            entry.mesh->shader->setBindMatrix3fv("normal_matrix", 1, 0, glm::value_ptr(normal_matrix));
            entry.mesh->draw();
        }
    }

    if (mesh) {
        updateAABB();
        //drawMeshAABB(view, projection);
        mesh->shader->useShader();
        mesh->shader->setMVPMatrix(model_matrix, view, projection);
        glm::mat3 normal_matrix = glm::inverse(glm::transpose(glm::mat3(model_matrix)));
        mesh->shader->setBindMatrix3fv("normal_matrix", 1, 0, glm::value_ptr(normal_matrix));
        mesh->draw();              
    }

    for (const auto child : children) {
        child->draw(view, projection, width, height);
    }
}

void SceneNode::computeShadow(std::shared_ptr<Shader> shadow_shader) {
    glm::mat4 model_matrix = get_world_transform();
    shadow_shader->useShader();
    shadow_shader->setBindMatrix4fv("model_matrix", 1, 0, glm::value_ptr(model_matrix));
    if(model) {
        for(const auto entry : model->entries) {
            entry.mesh->bindVAO();
            entry.mesh->drawElements();
        }
    }

    if(mesh) {
        mesh->bindVAO();
        mesh->drawElements();
    }

    for(const auto child : children) {
        child->computeShadow(shadow_shader);
    }
}

void SceneNode::updateAABB() {
    if(transform.transform_updated && mesh) {
        glm::mat4 model_matrix = get_world_transform();
        mesh->updateGlobalBoundingBox(model_matrix);
        transform.transform_updated = false;
    }
    if (transform.transform_updated && model) {
        glm::mat4 model_matrix = get_world_transform();
        model->updateGlobalBoundingBox(model_matrix);
        transform.transform_updated = false;
    }
}

void SceneNode::drawModelAABB(glm::mat4& view, glm::mat4& projection) {
    if (model) {
        std::shared_ptr<Shader> aabbShader = ShaderManager::getAABBShader();
        aabbShader->useShader();
        aabbShader->setVPMatrix(view, projection);
        model->bounding_box.drawBox();
    }
}

void SceneNode::drawMeshAABB(glm::mat4& view, glm::mat4& projection) {
    if(mesh) {
        std::shared_ptr<Shader> aabbShader = ShaderManager::getAABBShader();
        aabbShader->useShader();
        aabbShader->setVPMatrix(view, projection);
        mesh->bounding_box.drawBox();
    }
}

void SceneNode::enable_physics(bool use_gravity) {
    rigid_body->use_gravity = use_gravity;
}

void SceneNode::set_transform(glm::mat4 transform) {
    glm::vec3 scale;
    glm::quat quat;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, scale, quat, translation, skew, perspective);
    glm::vec3 rotation = glm::eulerAngles(quat);
    set_translation(translation);
    set_rotation(glm::degrees(rotation));
    set_scale(scale);
}

glm::vec3 SceneNode::get_position() { // Not working properly
    glm::mat4 transform_matrix = get_world_transform();
    glm::mat4 decompose;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::quat rotation;
    glm::decompose(decompose, scale, rotation, translation, skew, perspective);
    return translation;
}

glm::vec3 SceneNode::get_translation() {
    return transform.get_translation();
}

glm::vec3 SceneNode::get_rotation() {
    return transform.get_rotation();
}

glm::vec3 SceneNode::get_scale() {
    return transform.get_scale();
}

void SceneNode::set_translation(glm::vec3 translation) {
    transform.set_translation(translation);
}

void SceneNode::adjust_translation(glm::vec3 delta) {
    transform.adjust_translation(delta);
}

void SceneNode::set_rotation(glm::vec3 rotation) {
    transform.set_rotation(rotation);
}

void SceneNode::adjust_rotation(glm::vec3 delta) {
    transform.adjust_rotation(delta);
}

void SceneNode::set_scale(glm::vec3 scale) {
    transform.set_scale(scale);
}

void SceneNode::adjust_scale(glm::vec3 delta) {
    transform.adjust_scale(delta);
}
//