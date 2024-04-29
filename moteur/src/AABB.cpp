#include <AABB.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <climits>

AABB::AABB() {}

AABB::AABB(const glm::vec3 vmin, const glm::vec3 vmax) {
    min = vmin;
    max = vmax;
    originalMin = vmin;
    originalMax = vmax;
}

void AABB::processAABB(const glm::vec3 min, const glm::vec3 max) {
    this->min = min;
    this->max = max;
    this->originalMin = min;
    this->originalMax = max;
}

void AABB::drawBox() {
    GLfloat vertices[] = {
    // Bas de la boîte
    min.x, min.y, min.z,  max.x, min.y, min.z,
    max.x, min.y, min.z,  max.x, max.y, min.z,
    max.x, max.y, min.z,  min.x, max.y, min.z,
    min.x, max.y, min.z,  min.x, min.y, min.z,
    // Haut de la boîte
    min.x, min.y, max.z,  max.x, min.y, max.z,
    max.x, min.y, max.z,  max.x, max.y, max.z,
    max.x, max.y, max.z,  min.x, max.y, max.z,
    min.x, max.y, max.z,  min.x, min.y, max.z,
    // Côtés de la boîte
    min.x, min.y, min.z,  min.x, min.y, max.z,
    max.x, min.y, min.z,  max.x, min.y, max.z,
    min.x, max.y, min.z,  min.x, max.y, max.z,
    max.x, max.y, min.z,  max.x, max.y, max.z
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 24); // 24 sommets au total
    glBindVertexArray(0);
};
