#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Transform.hpp>
#include <glm/trigonometric.hpp>

Transform::Transform() {
    this->translation = glm::vec3(0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
}

Transform::Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
    this->translation = translation;
    this->rotation = rotation;
    this->scale = scale;
}

void Transform::adjust_translation(glm::vec3 delta) {
    this->translation += delta;
    this->transform_updated = true;
}

void Transform::adjust_rotation(glm::vec3 delta) {
    this->rotation += delta;
    this->transform_updated = true;
}

void Transform::adjust_scale(glm::vec3 delta) {
    this->scale += delta;
    this->transform_updated = true;
}

void Transform::set_translation(glm::vec3 translation) {
    this->translation = translation;
    this->transform_updated = true;
}

void Transform::set_rotation(glm::vec3 rotation) {
    this->rotation = rotation;
    this->transform_updated = true;
}

void Transform::set_scale(glm::vec3 scale) {
    this->scale = scale;
    this->transform_updated = true;
}

glm::mat4 Transform::get_matrix() {
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat,translation);
    mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    mat = glm::scale(mat,scale);
    return mat;
}