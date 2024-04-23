#include "SceneNode.hpp"
#include <PlaneCollider.hpp>

PlaneCollider::PlaneCollider(const Plane &plane) : plane(plane) {}

void PlaneCollider::check_collision_with_sphere(Sphere &sphere) {
    glm::vec3 spherePosition = sphere.getCenter();
    float current_height = plane.get_height_at_position(spherePosition.x,spherePosition.z);
    if(current_height >= spherePosition.y - sphere.getRadius()) {
        glm::vec3 newPosition = glm::vec3(spherePosition.x, current_height + sphere.getRadius(), spherePosition.z);
        sphere.setCenter(newPosition);
        glm::vec3 newVelocity = sphere.getVelocity();
        newVelocity.y = 0;
        sphere.setVelocity(newVelocity);
    }
}

void PlaneCollider::check_collision_with_cube(Cube &cube) {
    glm::vec3 cubePosition = cube.getCenter();
    //std::cout<<"Position inside collider : "<<cubePosition.y<<std::endl;
    float cubeBaseY = cubePosition.y - cube.getHalfSize();  
    float currentHeight = plane.get_height_at_position(cubePosition.x, cubePosition.z);
    if (currentHeight >= cubeBaseY) {
        glm::vec3 newPosition = glm::vec3(cubePosition.x, currentHeight + cube.getHalfSize(), cubePosition.z);
        cube.setCenter(newPosition);
        glm::vec3 newVelocity = cube.getVelocity();
        newVelocity.y = 0;  
        cube.setVelocity(newVelocity);
    }
}