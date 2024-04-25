#include <glm/gtc/constants.hpp>

#include <Sphere.hpp>

Sphere::Sphere(glm::vec3 center, float radius, unsigned int n_x, unsigned int n_y) {
    this->center = center;
    this->radius = radius;
    build_sphere(center, radius, n_x, n_y);
    setup_mesh();
}

void Sphere::update(float delta_time) {
    glm::vec3 gravity = glm::vec3(0.0f,-9.81f,0.0f);
    glm::vec3 acceleration = gravity;
    velocity += acceleration * delta_time;
    center += velocity * delta_time;
}

void Sphere::build_sphere(glm::vec3 center, float radius, unsigned int n_x, unsigned int n_y) {
    for (unsigned int i = 0; i <= n_x; ++i) {
        float V = (float)i / (float)n_x;
        float phi = V * glm::pi<float>();

        for (unsigned int j = 0; j <= n_y; ++j) {
            float U = (float)j / (float)n_y;
            float theta = U * (glm::pi<float>() * 2);

            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            // Adjust positions by the center
            Vertex vertex;
            vertex.position = center + glm::vec3(x, y, z) * radius;
            vertex.normal = glm::normalize(vertex.position - center);
            float u = theta / (2 * glm::pi<float>());
            float v = phi / glm::pi<float>();
            vertex.uv = glm::vec2(u, v);
            this->vertices.push_back(vertex);
            }
    }

    // Indices
    for (unsigned int i = 0; i < n_y * n_x + n_y; ++i) {
        this->indices.push_back(i);
        this->indices.push_back(i + n_y + 1);
        this->indices.push_back(i + n_y);

        this->indices.push_back(i + n_y + 1);
        this->indices.push_back(i);
        this->indices.push_back(i + 1);
    }
}