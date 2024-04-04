#pragma once

#include <Mesh.hpp>

class Plane : public Mesh {
    public:
        Plane(unsigned int grid_x = 10, unsigned int grid_z = 10, unsigned int size = 10, unsigned int height_scale=1);
        float get_height_at_position(float x, float z) const;
    
    private:
        unsigned int size;
        float x;
        float z;
        unsigned int grid_x;
        unsigned int grid_z;
        unsigned int height_scale;
        void setup_plane();
};