#include <cstdlib>
#include <ctime>

#include <Plane.hpp>

Plane::Plane(unsigned int grid_x, unsigned int grid_y) {
    this->grid_x = grid_x;
    this->grid_y = grid_y;
    setup_plane(grid_x, grid_y);
}

void Plane::setup_plane(unsigned int grid_x, unsigned int grid_y) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    float gridSizeX = 1.0f / grid_x;
    float gridSizeY = 1.0f / grid_y;

    for (unsigned int y = 0; y <= grid_y; ++y) {
        for (unsigned int x = 0; x <= grid_x; ++x) {
            Vertex vertex;
            vertex.position = glm::vec3(x * gridSizeX - 0.5f,(std::rand() % 20) / 100.0f, y * gridSizeY - 0.5f); // Random Y entre 0 et 0.2
            vertex.normal = glm::vec3(0, 1, 0); // Normale vers le haut
            vertex.uv = glm::vec2(x * gridSizeX, y * gridSizeY);
            this->vertices.push_back(vertex);
        }
    }

    for (unsigned int y = 0; y < grid_y; ++y) {
        for (unsigned int x = 0; x < grid_x; ++x) {
            this->indices.push_back((y    ) * (grid_x + 1) + x);
            this->indices.push_back((y + 1) * (grid_x + 1) + x);
            this->indices.push_back((y + 1) * (grid_x + 1) + x + 1);

            this->indices.push_back((y    ) * (grid_x + 1) + x);
            this->indices.push_back((y + 1) * (grid_x + 1) + x + 1);
            this->indices.push_back((y    ) * (grid_x + 1) + x + 1);
        }
    }

    this->setup_mesh();
}

float Plane::get_height_at_position(float x, float z) const {
    int x1 = floor(x);
    int x2 = ceil(x);
    int z1 = floor(z);
    int z2 = floor(z);

    float y11 = this->vertices[z1 * grid_x + x1].position.y;
    float y12 = this->vertices[z1 * grid_x + x2].position.y;
    float y21 = this->vertices[z2 * grid_x + x1].position.y;
    float y22 = this->vertices[z2 * grid_x + x2].position.y;

    float y1 = y11 + (x - x1) * y12;
    float y2 = y21 + (x - x1) * y22;

    float final_height = y1 + (z - z1) * y2;

    return final_height;
}