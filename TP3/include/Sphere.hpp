#pragma once

#include <Mesh.hpp>

class Sphere : public Mesh {
    public:
        // Constructor
        Sphere(glm::vec3 center, float radius, unsigned int n_x, unsigned int n_y);

    private:
        void build_sphere(glm::vec3 center,float radius, unsigned int n_x, unsigned int n_y);
};