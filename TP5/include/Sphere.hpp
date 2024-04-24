#pragma once

#include <Mesh.hpp>

class Sphere : public Mesh {
    public:
        // Constructor
        Sphere(glm::vec3 center, float radius, unsigned int n_x, unsigned int n_y);
        glm::vec3 getCenter() const {return center;}
        float getRadius() const {return radius;}
        glm::vec3 getVelocity() const {return velocity;}
        void setCenter(glm::vec3 center) {this->center = center;}
        void setRadius(float radius) {this->radius = radius;}
        void setVelocity(glm::vec3 velocity) {this->velocity = velocity;}
        void update(float delta_time);
        void launchSphere(glm::vec3 position, glm::vec3 direction, float speed);

    private:
        glm::vec3 center;
        float radius;
        glm::vec3 velocity;
        float mass;
        void build_sphere(glm::vec3 center,float radius, unsigned int n_x, unsigned int n_y);
};