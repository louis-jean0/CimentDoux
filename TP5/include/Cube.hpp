#pragma once

#include <SceneNode.hpp>
#include <Mesh.hpp>

class Cube : public Mesh {
public:
    // Constructor
    Cube(glm::vec3 center, float size);
    glm::vec3 getCenter() const {return this->center;}
    float getSize() const {return this->size;}
    float getHalfSize() const {return this->size/2.0f;}
    void setCenter(glm::vec3 center) {this->center = center;}
    void setSize(float size) {this->size = size;}
    void update(float delta_time);
    void setVelocity(glm::vec3 velocity) {this->velocity = velocity;}
    glm::vec3 getVelocity() const {return this->velocity;}
    void launchCube(glm::vec3 position, glm::vec3 direction, float speed);
    
private:
    glm::vec3 center;
    float size;
    glm::vec3 velocity;
    float mass;
    void build_cube(glm::vec3 center, float size);
};