#include <Plane.hpp>
#include <SceneNode.hpp>
#include <Cube.hpp>
#include <Sphere.hpp>

class PlaneCollider {
    public:
        // Constructor
        PlaneCollider(const Plane &plane);
        PlaneCollider(const Plane &plane, float friction_coefficient);

        // Public method
        void check_collision_with_sphere(Sphere &sphere);
        void check_collision_with_cube(Cube &cube);
        float getFrictionCoefficient() const {return this->friction_coefficient;}
        float getRestitutionCoefficient() const {return this->restitution_coefficient;}
        void setFrictionCoefficient(float friction_coefficient) {this->friction_coefficient = friction_coefficient;}
        void setRestitutionCoefficient(float restitution_coefficient) {this->restitution_coefficient = restitution_coefficient;} 
    
    private:
        // Private attributes
        const Plane &plane;
        float friction_coefficient = 0.5f;
        float restitution_coefficient = 0.1f;
};