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

void AABB::updateAABB(const glm::mat4& modelMatrix) {
    glm::vec3 minWorld = glm::vec3(modelMatrix * glm::vec4(originalMin, 1.0));
    glm::vec3 maxWorld = glm::vec3(modelMatrix * glm::vec4(originalMax, 1.0));

    glm::vec3 corners[8] = {
        glm::vec3(minWorld.x, minWorld.y, minWorld.z),
        glm::vec3(maxWorld.x, minWorld.y, minWorld.z),
        glm::vec3(minWorld.x, maxWorld.y, minWorld.z),
        glm::vec3(maxWorld.x, maxWorld.y, minWorld.z),
        glm::vec3(minWorld.x, minWorld.y, maxWorld.z),
        glm::vec3(maxWorld.x, minWorld.y, maxWorld.z),
        glm::vec3(minWorld.x, maxWorld.y, maxWorld.z),
        glm::vec3(maxWorld.x, maxWorld.y, maxWorld.z)
    };

    glm::vec3 newMin = corners[0];
    glm::vec3 newMax = corners[0];

    for (int i = 1; i < 8; ++i) {
        newMin = glm::min(newMin, corners[i]);
        newMax = glm::max(newMax, corners[i]);
    }
    min = newMin;
    max = newMax;
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

glm::vec3 AABB::getCenter() {
    return glm::vec3((min.x + max.x)/2, (min.y + max.y)/2, (min.z + max.z)/2);
}

glm::vec3 AABB::getHalfWidths() {
    return glm::vec3((max.x - min.x)/2, (max.y - min.y)/2, (max.z - min.z)/2);
}
