#include <SceneNode.hpp>
#include <Cube.hpp>
#include <glm/geometric.hpp>

Cube::Cube(glm::vec3 center, float size) {
    this->center = center;
    this->size = size;
    build_cube(center,size);
    this->velocity = glm::vec3(0.0f);
    this->mass = 10.0f;
}

void Cube::update(float delta_time) {
    glm::vec3 gravity = glm::vec3(0.0f,-9.81f,0.0f);
    glm::vec3 acceleration = gravity;
    velocity += acceleration * delta_time;
    center += velocity * delta_time;
}

void Cube::launchCube(glm::vec3 position, glm::vec3 direction, float speed) {
    center = position;
    this->velocity = glm::normalize(direction) * speed;
}

void Cube::build_cube(glm::vec3 center, float size) {
    // Half the size to center the cube around the origin
    float halfSize = size / 2.0f;

    // Vertices of a cube
    std::vector<Vertex> vertices = {
        // Front face
        {{center.x - halfSize, center.y - halfSize,  center.z + halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{center.x + halfSize, center.y - halfSize,  center.z + halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{center.x + halfSize, center.y + halfSize,  center.z + halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
        {{center.x - halfSize, center.y + halfSize,  center.z + halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Top-left

        // Back face
        {{center.x - halfSize, center.y - halfSize, center.z - halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{center.x - halfSize, center.y + halfSize, center.z - halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{center.x + halfSize, center.y + halfSize, center.z - halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{center.x + halfSize, center.y - halfSize, center.z - halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},

        // Left face
        {{center.x - halfSize, center.y - halfSize, center.z - halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{center.x - halfSize, center.y + halfSize, center.z - halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{center.x - halfSize, center.y + halfSize, center.z + halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{center.x - halfSize, center.y - halfSize, center.z + halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Right face
        {{center.x + halfSize, center.y - halfSize, center.z - halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{center.x + halfSize, center.y - halfSize, center.z + halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{center.x + halfSize, center.y + halfSize, center.z + halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{center.x + halfSize, center.y + halfSize, center.z - halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

        // Top face
        {{center.x - halfSize, center.y + halfSize, center.z - halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{center.x - halfSize, center.y + halfSize, center.z + halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{center.x + halfSize, center.y + halfSize, center.z + halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{center.x + halfSize, center.y + halfSize, center.z - halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},

        // Bottom face
        {{center.x - halfSize, center.y - halfSize, center.z - halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{center.x + halfSize, center.y - halfSize, center.z - halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{center.x + halfSize, center.y - halfSize, center.z + halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{center.x - halfSize, center.y - halfSize, center.z + halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}
    };

    // Indices for the vertices order
    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        8, 9, 10, 10, 11, 8,
        // Right
        12, 13, 14, 14, 15, 12,
        // Top
        16, 17, 18, 18, 19, 16,
        // Bottom
        20, 21, 22, 22, 23, 20
    };

    this->vertices = vertices;
    this->indices = indices;
    setup_mesh();
}