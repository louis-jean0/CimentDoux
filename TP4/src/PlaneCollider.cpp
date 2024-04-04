#include <PlaneCollider.hpp>

PlaneCollider::PlaneCollider(const Plane &plane) : plane(plane) {}

void PlaneCollider::check_collision_with_sphere(float sphere_radius, glm::vec3 &sphere_position) {
    float current_height = plane.get_height_at_position(sphere_position.x,sphere_position.z);
    //std::cout<<plane.vertices[0].position.y<<std::endl;
    //std::cout<<current_height<<std::endl;
    // if(current_height > sphere_position.y) {
    //     sphere_position.y = current_height + sphere_radius;
    // }
    sphere_position.y = current_height + sphere_radius;
}