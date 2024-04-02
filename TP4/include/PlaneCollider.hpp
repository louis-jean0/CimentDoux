#include <Plane.hpp>

class PlaneCollider {
    public:
        // Constructor
        PlaneCollider(const Plane &plane);

        // Public method
        void check_collision_with_sphere(float sphere_radius, glm::vec3 &sphere_position);
    
    private:
        // Private attributes
        const Plane &plane;
};