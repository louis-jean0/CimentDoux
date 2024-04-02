#pragma once

#include <Mesh.hpp>

class Plane : public Mesh {
    public:
        Plane(unsigned int grid_x = 1, unsigned int grid_y = 1);
        float get_height_at_position(float x, float y) const;
    
    private:
        int grid_x;
        int grid_y;
        void setup_plane(unsigned int grid_x, unsigned int grid_y);
};