#include <cstdlib>
#include <ctime>

#include <Plane.hpp>

Plane::Plane(unsigned int grid_x, unsigned int grid_z, unsigned int size, unsigned int height_scale) {
    this->size = size;
    this->grid_x = grid_x;
    this->grid_z = grid_z;
    this->x = 0;
    this->z = 0;
    this->height_scale = height_scale;
    setup_plane();
}

void Plane::setup_plane() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // Vertices data
    for (unsigned int z = 0; z <= grid_z; ++z) {
        for (unsigned int x = 0; x <= grid_x; ++x) {
            Vertex vertex;
            float real_x = this->x + x * (size / grid_x);
            float real_z = this->z + z * (size / grid_z);
            vertex.position = glm::vec3(real_x, (std::rand() % 100 / 100.0f) * height_scale, real_z);
            vertex.normal = glm::vec3(0, 1, 0); // Normale vers le haut
            vertex.uv = glm::vec2(x / (float)grid_x, z / (float)grid_z);
            this->vertices.push_back(vertex);
        }
    }
    // Indices
    for (unsigned int z = 0; z < grid_z; ++z) {
        for (unsigned int x = 0; x < grid_x; ++x) {
            this->indices.push_back((z    ) * (grid_x + 1) + x);
            this->indices.push_back((z + 1) * (grid_x + 1) + x);
            this->indices.push_back((z + 1) * (grid_x + 1) + x + 1);

            this->indices.push_back((z    ) * (grid_x + 1) + x);
            this->indices.push_back((z + 1) * (grid_x + 1) + x + 1);
            this->indices.push_back((z    ) * (grid_x + 1) + x + 1);
        }
    }
    this->setup_mesh();
}

float Plane::get_height_at_position(float x, float z) const {
    float terrain_x = x - this->x;
    float terrain_z = z - this->z;
    // std::cout<<"this->x "<<this->x<<std::endl;
    // std::cout<<"this->z "<<this->z<<std::endl;
    // std::cout<<"terrain_x "<<terrain_x<<std::endl;
    // std::cout<<"terrain_z "<<terrain_z<<std::endl;
    float grid_square_size_x = this->size / (float)(grid_x);
    float grid_square_size_z = this->size / (float)(grid_z);
    int pos_grid_x = (int)(terrain_x / (grid_square_size_x));
    int pos_grid_z = (int)(terrain_z / (grid_square_size_z));
    // std::cout<<"pos_grid_x "<<pos_grid_x<<"   "<<"grid_x "<<grid_x<<std::endl;
    // std::cout<<"pos_grid_z "<<pos_grid_z<<"   "<<"grid_z "<<grid_z<<std::endl;
    if(pos_grid_x > grid_x - 1|| pos_grid_z > grid_z - 1 || pos_grid_x < 0 || pos_grid_z < 0) {
        return 0;
    }
    float x_inside_square = fmod(terrain_x, grid_square_size_x) / grid_square_size_x;
    float z_inside_square = fmod(terrain_z, grid_square_size_z) / grid_square_size_z;

    float height00 = this->vertices[pos_grid_z * (grid_x + 1) + pos_grid_x].position.y;
    float height10 = this->vertices[pos_grid_z * (grid_x + 1) + pos_grid_x + 1].position.y;
    float height01 = this->vertices[(pos_grid_z + 1) * (grid_x + 1) + pos_grid_x].position.y;
    float height11 = this->vertices[(pos_grid_z + 1) * (grid_x + 1) + pos_grid_x + 1].position.y;

    float interpolated_height = height00 * (1 - x_inside_square) * (1 - z_inside_square) +
                                height10 * x_inside_square * (1 - z_inside_square) +
                                height01 * (1 - x_inside_square) * z_inside_square +
                                height11 * x_inside_square * z_inside_square;
                            
    //std::cout<<interpolated_height<<std::endl;

    return interpolated_height;
}