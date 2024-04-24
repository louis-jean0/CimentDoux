#include "SceneNode.hpp"
#include <PlaneCollider.hpp>

PlaneCollider::PlaneCollider(const Plane &plane) : plane(plane) {}

PlaneCollider::PlaneCollider(const Plane &plane, float friction_coefficient) : plane(plane), friction_coefficient(friction_coefficient) {}

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
    float cubeBaseY = cubePosition.y - cube.getHalfSize();
    float currentHeight = plane.get_height_at_position(cubePosition.x, cubePosition.z);

    if (currentHeight >= cubeBaseY) {
        glm::vec3 newPosition = glm::vec3(cubePosition.x, currentHeight + cube.getHalfSize(), cubePosition.z);
        cube.setCenter(newPosition);

        glm::vec3 newVelocity = cube.getVelocity();

        // Inversement et réduction de la vitesse y selon le coefficient de restitution
        newVelocity.y = -newVelocity.y * restitution_coefficient;

        // Application de la friction sur les composantes x et z
        newVelocity.x *= (1.0 - friction_coefficient);
        newVelocity.z *= (1.0 - friction_coefficient);

        cube.setVelocity(newVelocity);
    }
}
