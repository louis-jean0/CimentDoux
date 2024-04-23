#include <Plane.hpp>
#include <SceneNode.hpp>
#include <Cube.hpp>
#include <Sphere.hpp>

class PlaneCollider {
    public:
        // Constructor
        PlaneCollider(const Plane &plane);

        // Public method
        void check_collision_with_sphere(Sphere &sphere);
        void check_collision_with_cube(Cube &cube);
    
    private:
        // Private attributes
        const Plane &plane;
};