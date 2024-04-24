#include <glm/gtc/constants.hpp>

#include <Sphere.hpp>

Sphere::Sphere(glm::vec3 center, float radius, unsigned int n_x, unsigned int n_y) {
    build_sphere(center, radius, n_x, n_y);
    setup_mesh();
}

void Sphere::build_sphere(glm::vec3 center, float radius, unsigned int n_x, unsigned int n_y) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned short> indices;
    std::vector<glm::vec2> uvs;

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
            glm::vec3 vertex = center + glm::vec3(x, y, z) * radius;
            vertices.push_back(vertex);
            normals.push_back(glm::normalize(vertex - center));
            float u = theta / (2 * glm::pi<float>());
            float v = phi / glm::pi<float>();
            uvs.push_back(glm::vec2(u, v));
            }
    }

    // Indices
    for (unsigned int i = 0; i < n_y * n_x + n_y; ++i) {
        indices.push_back(i);
        indices.push_back(i + n_y + 1);
        indices.push_back(i + n_y);

        indices.push_back(i + n_y + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;
}